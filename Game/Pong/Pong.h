#pragma once

#define DLTA_INTEGRATED_GRAPHICS

#include <Dlta/Dlta.h>

using namespace dlta;

// TODO: just calculate the editor size based on the provided window size
#if DLTA_EDITOR
const int WIDTH = 1440;
const int HEIGHT = 810;
#else
const int WIDTH = 800;
const int HEIGHT = 600;
#endif

Mesh makePlane() {
	Mesh plane;

	plane.layout = { Vattr::XY, Vattr::UV };
	plane.indexed = true;
	plane.vertices = {
		// bottom left
		-.5f,-.5f, 0,0,
		// top left
		-.5f,.5f, 0,1,
		// bottom right
		.5f,-.5f, 1,0,
		// top right
		.5f,.5f, 1,1,
	};

	plane.indices = {
		0,1,2,
		1,3,2,
	};

	plane.create();

	return move(plane);
}

const int winscore = 8;
int score1 = 0;
int score2 = 0;

const float pspeed = 600;

class Paddle : public Entity {

public:
	RectTransform tf;
	SpriteRenderer renderer;
	Sprite& spr;

	Paddle(Shader& shader, Sprite& spr) : spr(spr), tf(RectTransform()), renderer(SpriteRenderer(shader, tf, spr)) {
		tf.position = vec2(-260, 0);
		tf.size = vec2(32, 128);
	}

	virtual void start() {
	}

	virtual void update() {

		vec2 move = vec2(0, pspeed * Time::deltaTime);
		if (Input::Key(Keycode::W)) tf.position += move;
		if (Input::Key(Keycode::S)) tf.position -= move;

		tf.position.y = math::clamp(tf.position.y, -250.f, 250.f);
	}
};

class AiPaddle : public Paddle {
public:
	RectTransform* ball = nullptr;

	float vision = 200;
	float speedMultiplier = .75;

	AiPaddle(Shader& shader, Sprite& spr) : Paddle(shader, spr) {
		tf.size = vec2(32, 128);
		tf.position = vec2(260, 0);
	}

	virtual void start()  override { }

	virtual void update() override {
		if (!ball) return;

		auto ballPos = ball->position;
		float direction;


		/*Gizmos::SetColor(Color::green);
		Gizmos::wireCircle(ballPos, 40.f);
		Gizmos::SetColor(Color::red);
		Gizmos::line(ballPos, ballPos + vec2(0, direction) * 30.f);*/

		bool ballClose = math::abs(ballPos.x - tf.position.x) < vision;
		if (ballClose) {
			direction = ballPos.y > tf.position.y ? 1 : -1;
		}
		else {
			direction = tf.position.y < -10 ? 1 : tf.position.y > 10 ? -1 : 0;
		}

		tf.position.y += direction * pspeed * speedMultiplier * Time::deltaTime;
		tf.position.y = math::clamp(tf.position.y, -250.f, 250.f);
		//tf.position.y = ballPos.y;
	}
};

class Ball : public Entity {
public:
	RectTransform tf;
	SpriteRenderer renderer;
	Sprite& spr;
	vec2 direction;

	RectTransform& paddle1;
	RectTransform& paddle2;
	float* timeout;
	float startSpeed = 400;
	float speed;
	float speedAdd = 40;

	Ball(Shader& shader, Sprite& spr, RectTransform& paddle1, RectTransform& paddle2, float* timeout) :
		spr(spr), tf(RectTransform()), renderer(SpriteRenderer(shader, tf, spr)),
		paddle1(paddle1), paddle2(paddle2), timeout(timeout) {
		auto r = math::random() + .2;
		auto s = math::random() + .2;
		cout << "the randoms: " << r << " " << s << endl;
		direction = glm::normalize(vec2(r, s));
		tf.position = vec2(0, 0);
		tf.size = vec2(32, 32);
		speed = startSpeed;
	}

	void start() {}

	enum class Collision {
		None,
		Paddle,
		WallHorizontal,
		WallVertical,
	};

	void update() {
		if (*timeout > 0) return;

		auto delta = direction * Time::deltaTime * speed;
		tf.position += delta;

		auto c = colliding();
		if (c != Collision::None) {
			auto randomDirection = glm::normalize(vec2(math::random(), math::random()));

			tf.position -= delta;
			switch (c) {
			case Collision::WallHorizontal:
				direction.y = -direction.y;
				direction.y += math::sign(direction.y) * math::random() * .1f;
				break;

			case Collision::WallVertical: {
				bool playerScore = tf.position.x > 0;
				if (playerScore) {
					score1++;
				}
				else {
					score2++;
				}

				speed = startSpeed;

				cout << "The score is " << score1 << " - " << score2 << endl;

				tf.position = vec2(0, 0);
				if (score1 < winscore && score2 < winscore) *timeout = 2;
			} break;

			case Collision::Paddle:
				speed += speedAdd;

				direction = -direction;
				direction.y += math::random() * .9;
				direction.x += math::random() * .9;

				/*if (math::abs(direction.y) > math::abs(direction.x)) {
					float newY = math::abs(direction.x) * math::sign(direction.y);
					float newX = math::abs(direction.y) * math::sign(direction.x);
					direction.x = newX;
					direction.y = newY;
				}*/

				break;
			}

			// introduce a bit of randomness
			direction.x = math::sign(direction.x) * math::max(math::abs(direction.x), .5f);
			direction = normalize(direction);
		}
	}


	Collision colliding() {
		if (math::abs(tf.position.x) > 400) return Collision::WallVertical;
		if (math::abs(tf.position.y) > 300) return Collision::WallHorizontal;
		if (collideWithRect(paddle1) || collideWithRect(paddle2)) return Collision::Paddle;
		return Collision::None;
	}

	bool collideWithRect(RectTransform& r) {
		auto p1 = tf.position;

		auto p2 = r.position;
		auto s2 = r.size;

		auto max2 = vec2(p2.x + s2.x / 2, p2.y + s2.y / 2);
		auto min2 = vec2(p2.x - s2.x / 2, p2.y - s2.y / 2);
		return p1.x > min2.x && p1.y > min2.y && p1.x < max2.x&& p1.y < max2.y;
	}
};

class Button {
public:
	RectTransform rt;
	Shader& shader;
	Sprite& sprite;
	SpriteRenderer renderer;

	Button(Shader& shader, Sprite& sprite) : rt(RectTransform()), shader(shader), sprite(sprite), renderer(shader, rt, sprite) {}

	bool clicked() {
		auto p = Input::mouse - vec2(400, 300);

		return point_in_box(p) && Input::MouseDown(MouseButton::Left);
	}

	bool point_in_box(vec2 p) {
		return
			(p.x <= rt.position.x + rt.size.x / 2) &&
			(p.x >= rt.position.x - rt.size.x / 2) &&
			(p.y <= rt.position.y + rt.size.y / 2) &&
			(p.y >= rt.position.y - rt.size.y / 2);
	}
};

class Image {
public:
	RectTransform rt;
	Shader& shader;
	Sprite& sprite;
	SpriteRenderer renderer;

	Image(Shader& shader, Sprite& sprite) : rt(RectTransform()), shader(shader), sprite(sprite), renderer(shader, rt, sprite) {}
};

class Pong : public Entity {
public:
	enum class GameState {
		StartMenu,
		Game,
		EndScreen,
	};
	enum class Difficulty {
		Easy,
		Normal,
		Hard,
	};

	Application& game;
	GameState state = GameState::StartMenu;
	float timeout = 0;

	Shader paddleShader = Shader("assets/shaders/sprite.vert", "assets/shaders/sprite.frag");
	Transform camTransform;
	Camera cam = Camera(camTransform, Projection::Orthographic);

	Sprite pongSprite = Sprite("assets/pongts.png");
	Sprite paddleSprite = Sprite::slice(pongSprite, vec2(8, 8), vec2(0, 0), vec2(1, 4));
	Sprite ballSprite = Sprite::slice(pongSprite, vec2(8, 8), vec2(1, 0), vec2(1, 1));
	Sprite easyButtonSprite = Sprite::slice(pongSprite, vec2(8, 8), vec2(4, 0), vec2(4, 1));
	Sprite normalButtonSprite = Sprite::slice(pongSprite, vec2(8, 8), vec2(4, 1), vec2(4, 1));
	Sprite hardButtonSprite = Sprite::slice(pongSprite, vec2(8, 8), vec2(4, 2), vec2(4, 1));
	Sprite quitButtonSprite = Sprite::slice(pongSprite, vec2(8, 8), vec2(4, 3), vec2(4, 1));
	Sprite p1ScoreSprite = Sprite::slice(pongSprite, vec2(8, 8), vec2(4, 3), vec2(4, 1));
	Sprite p1winSprite = Sprite::slice(pongSprite, vec2(8, 8), vec2(0, 4), vec2(8, 1));
	Sprite scoreSprite = Sprite::slice(pongSprite, vec2(8, 8), vec2(0, 6), vec2(1, 1));
	Sprite scoreSprite2 = Sprite::slice(pongSprite, vec2(8, 8), vec2(0, 6), vec2(1, 1));
	Image winImg = Image(paddleShader, p1winSprite);
	Image score1Img = Image(paddleShader, scoreSprite);
	Image score2Img = Image(paddleShader, scoreSprite2);

	Paddle paddle = Paddle(paddleShader, paddleSprite);
	AiPaddle aipaddle = AiPaddle(paddleShader, paddleSprite);
	Ball ball = Ball(paddleShader, ballSprite, paddle.tf, aipaddle.tf, &timeout);

	Button easyButton = Button(paddleShader, easyButtonSprite);
	Button normalButton = Button(paddleShader, normalButtonSprite);
	Button hardButton = Button(paddleShader, hardButtonSprite);
	Button quitButton = Button(paddleShader, quitButtonSprite);

	Pong(Application& game) :game(game) {
		camTransform.position = vec3(0, 0, 1);
		camTransform.euler.y = -90;
		camTransform.euler.x = 0;

		cam.clearColor = Color::rgb(.1f, .1, .1);

		Camera::main = &cam;

		aipaddle.ball = &ball.tf;

		easyButton.rt.position = vec2(0, 80); easyButton.rt.size = vec2(128, 32);
		normalButton.rt.position = vec2(0, 40); normalButton.rt.size = vec2(128, 32);
		hardButton.rt.position = vec2(0, 0); hardButton.rt.size = vec2(128, 32);
		quitButton.rt.position = vec2(0, -40); quitButton.rt.size = vec2(128, 32);

		score1Img.rt.position = vec2(-100, 100); score1Img.rt.size = vec2(32, 32);
		score2Img.rt.position = vec2(100, 100); score2Img.rt.size = vec2(32, 32);

		winImg.rt.position = vec2(0, 0); winImg.rt.size = vec2(256, 32);

		changeState(state);
	}

	void start() {

	}

	void update() {
		if (timeout > 0) {
			(timeout) -= Time::deltaTime;
		}

		switch (state) {
		case GameState::StartMenu: {
			if (easyButton.clicked()) { changeState(GameState::Game, Difficulty::Easy); }
			if (normalButton.clicked()) { changeState(GameState::Game, Difficulty::Normal); }
			if (hardButton.clicked()) { changeState(GameState::Game, Difficulty::Hard); }
			if (quitButton.clicked()) { game.quit(); }
		} break;
		case GameState::Game: {
			score1Img.sprite.offset = offsetForScore(score1);
			score2Img.sprite.offset = offsetForScore(score2);

			if (score1 >= winscore || score2 >= winscore) {
				changeState(GameState::EndScreen);
			}
		} break;
		case GameState::EndScreen: {
			if (timeout <= 0) {
				// end immediately after timeout is complete
				cout << "end game!" << endl;
				changeState(GameState::StartMenu);
			}
		} break;
		}

	}

	vec2 offsetForScore(int score) {
		return vec2(0 + (score % 8), 6 + (score / 8));
	}

	void changeState(GameState s, Difficulty d = Difficulty::Easy) {
		// clear everything
		game.entities.clear();
		game.renderers2d.clear();

		// enable elements for this screen
		game.entities.push_back(this);
		switch (s) {
		case GameState::StartMenu: {
			score1 = 0;
			score2 = 0;
			paddle.tf.position.y = 0;
			aipaddle.tf.position.y = 0;
			game.renderers2d.push_back(&easyButton.renderer);
			game.renderers2d.push_back(&normalButton.renderer);
			game.renderers2d.push_back(&hardButton.renderer);
			game.renderers2d.push_back(&quitButton.renderer);
		} break;
		case GameState::Game: {
			timeout = 2;
			switch (d) {
			case Difficulty::Easy: {ball.startSpeed = 350; ball.speedAdd = 10; aipaddle.vision = 100; aipaddle.speedMultiplier = .7; } break;
			case Difficulty::Normal: {ball.startSpeed = 400; ball.speedAdd = 40; aipaddle.vision = 110; aipaddle.speedMultiplier = .75; } break;
			case Difficulty::Hard: {ball.startSpeed = 500; ball.speedAdd = 60; aipaddle.vision = 240; aipaddle.speedMultiplier = 1; } break;
			}
			game.entities.push_back(&paddle);
			game.entities.push_back(&aipaddle);
			game.entities.push_back(&ball);
			game.renderers2d.push_back(&paddle.renderer);
			game.renderers2d.push_back(&aipaddle.renderer);
			game.renderers2d.push_back(&ball.renderer);
			game.renderers2d.push_back(&score1Img.renderer);
			game.renderers2d.push_back(&score2Img.renderer);
		} break;
		case GameState::EndScreen: {
			timeout = 4;
			cout << (score1 > score2 ? "p1 wins" : "p2 wins") << endl;
			winImg.sprite.offset.y = (score1 > score2 ? 4 : 5);

			game.renderers2d.push_back(&score1Img.renderer);
			game.renderers2d.push_back(&score2Img.renderer);
			game.renderers2d.push_back(&winImg.renderer);
		} break;
		}

		state = s;
	}
};

int game() {
	Application game(WIDTH, HEIGHT, "Pong", false);

	Sprite::spriteQuad = makePlane();

	auto pongGame = Pong(game);

	game.run();

	return 0;
}
