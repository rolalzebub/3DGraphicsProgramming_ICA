#version 330
#define MAX_LIGHTS 8
in vec2 varying_texcoord;
in vec3 varying_normal;
in vec3 varying_position;
in mat3 model_xform_;
out vec3 fragment_colour;

uniform sampler2D u_texture;

uniform vec3 camera_position;

uniform bool shiny = true;
uniform float shininess = 0.5f;
uniform vec3 specular_colour = vec3(5);

uniform int numLights;

uniform struct Light{
	vec4 position;		//w=0 means directional Light, 1 = point light
	vec4 rotation;		//if position.w = 1, then w=0 means normal point light,	1= cone light
	
	vec3 colour_intensity;

	float attenuation;
	float ambientCoefficient;
	
	float coneAngle;
	vec3 coneDirection;
} 
lightSource[MAX_LIGHTS];

vec3 ApplyLight(Light light, vec3 surfaceColor, vec3 normal, vec3 surfacePos, vec3 surfaceToCamera) {

    vec3 surfaceToLight;
    float attenuation = 1.0;
    if(light.position.w == 0.0) {
        //directional light
        surfaceToLight = normalize(light.position.xyz);
        attenuation = 1.0; //no attenuation for directional lights
    } else {
        //point light
        surfaceToLight = normalize(light.position.xyz - surfacePos);
        float distanceToLight = length(light.position.xyz - surfacePos);
        attenuation = 1.0 / (1.0 + attenuation * pow(distanceToLight, 2));

		if(light.rotation.w != 0)
        {	//cone restrictions (affects attenuation)
			float lightToSurfaceAngle = degrees(acos(dot(-surfaceToLight, normalize(light.coneDirection))));
			if(lightToSurfaceAngle > light.coneAngle)
			{
				attenuation = 0.0;
			}
		}
    }

    //ambient
    vec3 ambient = light.ambientCoefficient * surfaceColor.rgb * light.colour_intensity;

    //diffuse
    float diffuseCoefficient = max(0.0, dot(normal, surfaceToLight));
    vec3 diffuse = diffuseCoefficient * surfaceColor.rgb * light.colour_intensity;
    
    //specular
	vec3 specular = vec3(0);
	if(shiny == true)
	{
		float specularCoefficient = 0.0;
			if(diffuseCoefficient > 0.0)
				specularCoefficient = pow(max(0.0, dot(surfaceToCamera, reflect(-surfaceToLight, normal))), shininess);
			specular = specularCoefficient * specular_colour * light.colour_intensity;
	}

    return ambient + attenuation*(diffuse + specular);
}

void main(void)
{
	vec3 texColour = texture(u_texture, varying_texcoord).rgb;
	
	mat3 normalMatrix = transpose(inverse(mat3(model_xform_)));
    vec3 normal = normalize(normalMatrix * varying_normal);

	vec3 final_colour = vec3(0);
	for(int i = 0; i < numLights; i++)
		final_colour += ApplyLight(lightSource[i], texColour, normal, varying_position, camera_position);

	fragment_colour = final_colour;
}