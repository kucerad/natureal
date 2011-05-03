#
//==============================================================================
//  Terrain FRAGMENT shader   
//
//==============================================================================
#define SCALE 1.0

uniform sampler2D terrain_tex_01;
uniform sampler2D terrain_tex_02;
uniform sampler2D terrain_tex_03;
uniform sampler2D terrain_tex_04;
uniform sampler2D terrain_tex_05;

varying vec3	eye;
varying vec3	normal;
varying float	height;

void main()
{
	vec3 N = normalize(normal);
	vec3 L = normalize(gl_LightSource[0].position.xyz- eye);
	vec3 E = normalize(-eye);
	vec3 R = normalize(-reflect(L,N));

	float NdotL = max(dot(normalize(N),normalize(L)), 0.0);
	float RdotE = max(dot(R,E),0.0);

	vec4 Ia = gl_FrontLightProduct[0].ambient;
	vec4 Id = gl_FrontLightProduct[0].diffuse * NdotL;
	//vec4 Is = gl_FrontLightProduct[0].specular * pow(RdotE, gl_FrontMaterial.shininess);
	vec4 Is = gl_FrontLightProduct[0].specular * pow(RdotE, 2.0);

	//-----------------------------------------------------------
	// multitexturing :

	vec4 tex_color, tex_color1, tex_color2;
	vec4 border_values	= vec4(10.0, 5.0, 2.0, 0.0);
	vec4 border_widths  = vec4(5.0, 2.0, 1.0, 0.5);


	if (height > border_values.x)
	{
		tex_color1 = texture2D(terrain_tex_01, gl_TexCoord[0].st*SCALE);
		tex_color2 = texture2D(terrain_tex_02, gl_TexCoord[0].st*SCALE);
		tex_color = mix(tex_color2, tex_color1, min(max((height - border_values.x)/border_widths.x, 0.0), 1.0));
	}
	else if (height > border_values.y)
	{
		tex_color1 = texture2D(terrain_tex_02, gl_TexCoord[0].st*SCALE);
		tex_color2 = texture2D(terrain_tex_03, gl_TexCoord[0].st*SCALE);
		tex_color = mix(tex_color2, tex_color1, min(max((height - border_values.y)/border_widths.y, 0.0), 1.0));
	}
	else if (height > border_values.z)
	{
		tex_color1 = texture2D(terrain_tex_03, gl_TexCoord[0].st*SCALE);
		tex_color2 = texture2D(terrain_tex_04, gl_TexCoord[0].st*SCALE);
		tex_color = mix(tex_color2, tex_color1, min(max((height - border_values.z)/border_widths.z, 0.0), 1.0));
	}
	else
	{
		tex_color1 = texture2D(terrain_tex_04, gl_TexCoord[0].st*SCALE);
		tex_color2 = texture2D(terrain_tex_05, gl_TexCoord[0].st*SCALE);
		tex_color = mix(tex_color2, tex_color1, min(max((height - border_values.w)/border_widths.w, 0.0), 1.0));
	}
	gl_FragColor = gl_FrontLightModelProduct.sceneColor + (Ia + Id)*tex_color +Is;
}