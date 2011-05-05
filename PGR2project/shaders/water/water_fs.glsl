#
//==============================================================================
//  Water FRAGMENT shader   
//  inspired by: http://forum.bonzaisoftware.com/viewthread.php?tid=10
//				 http://www.bonzaisoftware.com/water_tut.html#refl
//   
//               GLSL conversion of Michael Horsch water demo
//               http://www.bonzaisoftware.com/wfs.html
//
//               Converted by Mars_999
//               8/20/2005            
//==============================================================================


//uniform sampler2D water_normalmap;
uniform sampler2D water_reflection;
//uniform sampler2D water_refraction;
//uniform sampler2D water_dudvmap;
//uniform sampler2D water_depthmap;
//uniform vec4 waterColor;

//varying vec4 light_pos; //lightpos
//varying vec4 movement1; //moving texcoords
//varying vec4 movement2; //moving texcoords
varying vec4 projectedVertex; //for projection
//varying vec4 eyeVector; //viewts

//unit 0 = water_reflection
//unit 1 = water_refraction
//unit 2 = water_normalmap
//unit 3 = water_dudvmap
//unit 4 = water_depthmap

void main(void)
{
	//vec4 waterColor;
	//const vec4 sca = vec4(0.005, 0.005, 0.005, 0.005);
	//const vec4 sca2 = vec4(0.02, 0.02, 0.02, 0.02);
	//const vec4 tscale = vec4(0.25, 0.25, 0.25, 0.25);
	//const vec4 two = vec4(2.0, 2.0, 2.0, 1.0);
	//const vec4 mone = vec4(-1.0, -1.0, -1.0, 1.0);
	//
	//const vec4 ofive = vec4(0.5,0.5,0.5,1.0);
	//
	//const float exponent = 64.0;
	//
	//vec4 lightTS	    = normalize(light_pos);
	//vec4 viewt		= normalize(eyeVector);
	//vec4 disdis		= texture2D(water_dudvmap, vec2(movement2 * tscale));
	//vec4 dist		    = texture2D(water_dudvmap, vec2(movement1 + disdis*sca2));
	//vec4 fdist		= normalize(dist* two + mone)*sca;

	//load normalmap
		//vec4 nmap = texture2D(water_normalmap, vec2(movement1 + disdis*sca2));
		//nmap = nmap * two + mone;
		//vec4 vNorm = normalize(nmap);

	//get projective texcoords
	vec4 tmp = vec4(1.0 / projectedVertex.w);
	vec4 temp = tmp;

	vec4 projCoord = projectedVertex * tmp;
	projCoord += vec4(1.0);
	projCoord *= vec4(0.5);
	tmp = projCoord;
	//tmp = projCoord + fdist;
	tmp = clamp(tmp, 0.001, 0.999);

	//load reflection,refraction and depth texture
	vec4 refTex = texture2D(water_reflection, vec2(tmp));
	vec4 reflection = refTex;
	reflection.a = 0.5;

		//vec4 refr   = texture2D(water_refraction, vec2(tmp));
		//vec4 wdepth = texture2D(water_depthmap, vec2(tmp));

		//wdepth = vec4(pow(wdepth.x, 4.0));
		//vec4 invdepth = 1.0 - wdepth;

	//calculate specular highlight
		//vec4 vRef = normalize(reflect(-lightTS, vNorm));
		//float stemp = max(0.0, dot(viewt, vRef) );
		//stemp = pow(stemp, exponent);
		//vec4 specular = vec4(stemp);

	//calculate fresnel and inverted fresnel
		//vec4 invfres = vec4( dot(vNorm, viewt) );
		//vec4 fres = vec4(1.0) - invfres ;

	//calculate reflection and refraction
		//refr = refr * invfres * invdepth + (waterColor * wdepth * invfres);
		//refl *= fres;

	//add reflection and refraction
	gl_FragColor = reflection;
	//gl_FragColor = refr + refl + specular;
}