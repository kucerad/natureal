
varying vec3 vNormal;	// normala
varying vec4 vPos;		// pozice

attribute vec3 a_binormal;
attribute vec3 a_facenormal;

void main() 
{ 
	vec3 binormal	= a_binormal;
	vec3 facenormal = a_facenormal;
	 
    vPos			= gl_ModelViewMatrix*gl_Vertex;

	vNormal			= gl_NormalMatrix*vNormal;

	gl_TexCoord[0]	= gl_MultiTexCoord0;
	
	gl_FrontColor	= gl_Color;
	gl_Position		= gl_ProjectionMatrix * vPos;
}