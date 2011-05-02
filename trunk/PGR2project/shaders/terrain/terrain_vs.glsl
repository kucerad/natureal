#
//==============================================================================
//  Terrain VERTEX shader   
//
//==============================================================================
	
varying vec3	eye;
varying vec3	normal;
varying float	height;

void main()
{
	eye = vec3(gl_ModelViewMatrix * gl_Vertex);
	normal = gl_NormalMatrix * gl_Normal;
	height = gl_Vertex.y;
	
	gl_TexCoord[0] = gl_MultiTexCoord0;

	gl_FrontColor = gl_Color;
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}