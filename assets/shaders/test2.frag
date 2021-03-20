#version 330 core

in vec2 uv;
in vec3 ro;
in vec3 hitPos;

uniform float iTime;
uniform vec3 iCamera;
uniform vec3 iLight;
uniform vec3 iLightColor;
uniform sampler2D tex;
uniform sampler2D tex2;
uniform samplerCube skybox;

out vec4 FragColor;

#define MAX_STEPS 100
#define SURFACE_DIST 0.0001
#define MAX_DIST 1000

vec3 opRep( in vec3 p, in vec3 c)
{
    return mod(p+0.5*c,c)-0.5*c;
}

float sdBox(vec3 p, vec3 b )
{
  vec3 q = abs(p) - b;
  return length(max(q,0.0)) + min(max(q.x,max(q.y,q.z)),0.0);
}

float sdMandelbulb(in vec3 p, out vec4 resColor) {
	vec3 w = p;
    	float m = dot(w,w);

    	vec4 trap = vec4(abs(w),m);
		float dz = 1.0;
    
		for( int i=0; i<4; i++ )
    	{
	#if 0
        	// polynomial version (no trigonometrics, but MUCH slower)
        	float m2 = m*m;
        	float m4 = m2*m2;
			dz = 8.0*sqrt(m4*m2*m)*dz + 1.0;

        	float x = w.x; float x2 = x*x; float x4 = x2*x2;
        	float y = w.y; float y2 = y*y; float y4 = y2*y2;
        	float z = w.z; float z2 = z*z; float z4 = z2*z2;

        	float k3 = x2 + z2;
        	float k2 = inversesqrt( k3*k3*k3*k3*k3*k3*k3 );
        	float k1 = x4 + y4 + z4 - 6.0*y2*z2 - 6.0*x2*y2 + 2.0*z2*x2;
        	float k4 = x2 - y2 + z2;

        	w.x = p.x +  64.0*x*y*z*(x2-z2)*k4*(x4-6.0*x2*z2+z4)*k1*k2;
        	w.y = p.y + -16.0*y2*k3*k4*k4 + k1*k1;
        	w.z = p.z +  -8.0*y*k4*(x4*x4 - 28.0*x4*x2*z2 + 70.0*x4*z4 - 28.0*x2*z2*z4 + z4*z4)*k1*k2;
	#else
        	// trigonometric version (MUCH faster than polynomial)
        
        	// dz = 8*z^7*dz
			dz = 8.0*pow(m,3.5)*dz + 1.0;
      	//dz = 8.0*pow(sqrt(m),7.0)*dz + 1.0;
      
        	// z = z^8+z
        	float r = length(w);
        	float b = 8.0*acos( w.y/r) + iTime;
        	float a = 8.0*atan( w.x, w.z )+(iTime*2);
        	w = p + pow(r,8.0) * vec3( sin(b)*sin(a), cos(b), sin(b)*cos(a) );
	#endif        
        
        	trap = min( trap, vec4(abs(w),m) );

        	m = dot(w,w);
			if( m > 256.0 )
            	break;
    	}

    	resColor = vec4(m,trap.yzw);

    	// distance estimation (through the Hubbard-Douady potential)
    	return 0.25*log(m)*sqrt(m)/dz;
}

vec4 bulbColor;

float getDist(vec3 p) {
	//p.xz += vec2(sin(iTime+p.y*10), cos(iTime+p.y*10))*.2;

	// float amount = (mod(floor(iTime/2), 4))*2 + 1;

	// vec3 q =  opRep(p, vec3(1/amount));

	// float d = length(q)-(1/amount*.3);

	// return d*.7;
	p*=2;
	return sdMandelbulb(p, bulbColor);
}

vec3 getNormal(vec3 p) {
	vec2 e = vec2(1e-2, 0);
	vec3 n = getDist(p) - vec3(
		getDist(p-e.xyy),
		getDist(p-e.yxy),
		getDist(p-e.yyx)
	);

	return normalize(n);
}

vec2 march(vec3 ro, vec3 rd, float od) {
	float tolerance = 0.01;
	float dO = od;
	float dS=999;
	int i;
	for (i = 0; i< MAX_STEPS; i++) {
		vec3 p = ro + dO*rd;	
		float domain = sdBox(p, vec3(.5));
		if (domain>tolerance) break;
		dS = getDist(p);
		dO += dS;
		if (dS<SURFACE_DIST || dO > MAX_DIST) break;
	}

	if (dS>SURFACE_DIST) discard;

	return vec2(dO, float(i)/MAX_STEPS);
}

vec3 lighting(vec3 albedo, vec3 n, vec3 p) {
	vec3 lightDir = normalize(iLight - p);

	// ambient
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * iLightColor;

	// diffuse
	float diff = max(dot(n, lightDir), 0);
	vec3 diffuse = diff*iLightColor;

	// specular
	float specularStrength = 1.0;
	vec3 viewDir = normalize(iCamera - p);
	vec3 reflectDir = reflect(-lightDir, n);
	float spec = pow(max(dot(viewDir, reflectDir),0), 32);
	vec3 specular = specularStrength * spec * iLightColor;

	// reflection
	float reflectivity = 1.0;
	float glossiness = .95;
	const float lods = 8.;
	// vec3 i = normalize(p - iCamera);
	// vec3 r = reflect(i, n);
	// vec3 reflection = reflectivity * textureLod(skybox, r, lods-(lods*glossiness)).rgb;
	diffuse*=1-reflectivity;

	// refraction
	// Air	1.00
	// Water	1.33
	// Ice	1.309
	// Glass	1.52
	// Diamond	2.42
	float ratio = 1.00 / 1.52;
    vec3 I = normalize(p - iCamera);
    vec3 R = refract(I, normalize(n), ratio);
	vec3 refraction = reflectivity * textureLod(skybox, R, lods-(lods*glossiness)).rgb;

	 vec3 result = (ambient + diffuse + specular + refraction) * albedo;
	//  vec3 result = refraction;

	return result;
}

void main() {
	vec3 rr = hitPos-ro;
	float od = length(rr);
	vec3 rd = normalize(rr);

	vec3 col;

	vec2 m = march(ro, rd, od);
	float d = m.x;
	float s = m.y;

	vec3 p = ro+rd*d;
	vec3 n = getNormal(ro+rd*d);
	//  vec3 albedo = vec3(1,.5,.1);
	// vec3 albedo = vec3(1,1,1);
	//vec3 albedo = texture(tex, n.xy).xyz;
	col = lighting(vec3(1), n, p);

	FragColor = vec4(col, 1);
}

