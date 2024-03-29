#version 120

// PROPIEDADES DEL PUNTO
varying vec3 Position_eye;
varying vec3 LightDir;
varying vec2 TexCoord;
varying vec3 ViewDir;

// PROPIEDADES DE LA LUZ , LUZ DEL ENTORNO
uniform vec4 LightPosition; // Light position in eye coords.
uniform vec3 La; // Ambient light intensity
uniform vec3 Ld; // Diffuse light intensity
uniform vec3 Ls; // Specular light intensity

// PROPIEDADES DEL MATERIAL
uniform vec3 Ka; // Ambient reflectivity (0<= ka <=1)
uniform vec3 Kd; // Diffuse reflectivity
uniform vec3 Ks; // Specular reflectivity ( max es (1,1,1) min es (0,0,0) )
uniform float Shininess; // Specular shininess factor

// PROPIEDADES DE LA NIEBLA
uniform float FogMaxDist;
uniform float FogMinDist;
uniform vec3  FogColor;

// MATRICES
uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ProjectionMatrix;

// COORDENADA DE LA TEXTURA
uniform sampler2D Tex1;
uniform sampler2D NormalMapTex;

vec3 phongModel( vec3 norm, vec3 diffR ) {
	vec3 r = reflect( -LightDir, norm );
	r = normalize (r);
	vec3 ambient = La * Ka;
	float sDotN = max( dot(LightDir, norm), 0.0 );
	
	vec3 diffuse = Ld * diffR * sDotN;
	vec3 spec = vec3(0.0);
	if( sDotN > 0.0 )
		spec = Ls * Ks * pow( max( dot(r,ViewDir), 0.0 ), Shininess );
	return ambient + diffuse + spec;
}

vec3 getShadeColor () {
	// Lookup the normal from the normal map
	vec4 normal = texture2D (NormalMapTex, TexCoord);
	normal = normalize (normal);
	// The color texture is used as the diffuse reflectivity
	vec4 texColor = texture2D (Tex1, TexCoord);
	
	return phongModel (normal.xyz, texColor.rgb);
}

float getFogFactor (float positionZ) {
	float dist = abs (positionZ);
	float fogFactor = (FogMaxDist - dist) / (FogMaxDist - FogMinDist);
	return clamp (fogFactor, 0.0, 1.0);
}

void main() {
	// Calculo del fog factor
	float fogFactor = getFogFactor (Position_eye.z);
	
	// Se obtiene el color de la malla, se lo mezcla con la niebla y se construye el color del fragmento
	vec3 shadeColor = getShadeColor();
	vec3 color = mix (FogColor, shadeColor, fogFactor);
	gl_FragColor = vec4(color, 1.0);
}
