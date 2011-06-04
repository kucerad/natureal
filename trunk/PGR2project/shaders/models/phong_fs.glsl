
varying vec3 vNormal;
varying vec4 vPos;

uniform sampler2D diffuse_texture;

void main(){

	vec3 vLightDir = gl_LightSource[0].position.xyz - vPos.xyz;
	
	float h = gl_FrontMaterial.shininess;
	vec3 E = normalize(-vPos.xyz);
	vec3 R = normalize(-reflect(vLightDir, vNormal).xyz);
	float RdotE = max(dot(R, E),0.0);
	float NdotL = max(dot(normalize(vNormal), normalize(vLightDir)),0.0);
	vec4 texColor = texture2D(diffuse_texture, gl_TexCoord[0].st);
	
	vec4 color = texColor * (gl_FrontLightProduct[0].ambient + gl_FrontLightProduct[0].diffuse * NdotL) + gl_FrontLightProduct[0].specular * pow(RdotE,h);
	gl_FragData[0] = color;
	gl_FragData[1] = vec4(0.0, 0.0, 0.0, 1.0);
}
