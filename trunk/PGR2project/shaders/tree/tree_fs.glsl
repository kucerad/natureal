#
//==============================================================================
//  Tree1 FRAGMENT shader   
//
//==============================================================================
#define SCALE 0.1

uniform sampler2D tree1_tex;
varying vec3 normal;
varying vec3 eye;

void main()
{
	vec4 tex_color = texture2D(tree1_tex, gl_TexCoord[0].st);
	//-----------------------------------------------------------
	// 
	if (tex_color.a<0.1){
		discard;
	}
	vec3 N = normalize(normal);
	vec3 L = normalize(gl_LightSource[0].position.xyz - eye);
	//vec3 E = normalize(-eye);
	//vec3 R = normalize(-reflect(L,N));

	float NdotL = abs(dot(normalize(N),normalize(L)));
	//float RdotE = max(dot(R,E),0.0);

	vec4 Ia = gl_FrontLightProduct[0].ambient;
	vec4 Id = gl_FrontLightProduct[0].diffuse * NdotL;
	//vec4 Is = gl_FrontLightProduct[0].specular * pow(RdotE, gl_FrontMaterial.shininess);
	//vec4 Is = vec40.0; //gl_FrontLightProduct[0].specular * pow(RdotE, 2.0);
	
	vec4 color = gl_FrontLightModelProduct.sceneColor + (Ia + Id)*tex_color;
	color.a = tex_color.a;
	gl_FragData[0] = color;
	gl_FragData[1] = vec4(0.0, 0.0, 0.0, 1.0);
}