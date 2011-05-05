#
//==============================================================================
//  Water VERTEX shader   
//  inspired by: http://forum.bonzaisoftware.com/viewthread.php?tid=10
//				 http://www.bonzaisoftware.com/water_tut.html#refl
//==============================================================================
//varying vec4 light_pos;
//varying vec4 movement1;
//varying vec4 movement2;
varying vec4 projectedVertex;
//varying vec4 eyeVector;
//uniform vec4 viewpos, lightpos;
//uniform float time, time2;
//unit 0 = water_reflection
//unit 1 = water_refraction
//unit 2 = water_normalmap
//unit 3 = water_dudvmap
//unit 4 = water_depthmap
void main(void)
{
	vec4 temp;
	// create tangent space coordinate base system [FIXED]
		//vec4 tangent	= vec4(1.0, 0.0, 0.0, 0.0);
		//vec4 norm		= vec4(0.0, 1.0, 0.0, 0.0);
		//vec4 binormal	= vec4(0.0, 0.0, 1.0, 0.0);

	

	// light direction 
		//temp = gl_LightSource[0].position - gl_Vertex;
	// light dir in tangent space
		//light_pos.x = dot(temp, tangent);
		//light_pos.y = dot(temp, binormal);
		//light_pos.z = dot(temp, norm);
		//light_pos.w = 0.0;

		//vec4 t1 = vec4(0.0, -time, 0.0,0.0);
		//vec4 t2 = vec4(0.0, -time2, 0.0,0.0);

		//mat4 mtx = gl_TextureMatrix[0]; // maybe useful below...
	// refraction vector
		//movement1 = gl_MultiTexCoord0 + t1;
	// normal vector
		//movement2 = gl_MultiTexCoord0 + t2;

	// dudv vector
	projectedVertex = gl_ModelViewProjectionMatrix * gl_Vertex;

	// eye vector
		//temp = - (gl_ModelVIewMatrix * gl_Vertex);
	// eye vector in tangent space
		//eyeVector.x = dot(temp, tangent);
		//eyeVector.y = dot(temp, binormal);
		//eyeVector.z = dot(temp, norm);
		//eyeVector.w = 0.0;

	gl_Position = projectedVertex;
}


