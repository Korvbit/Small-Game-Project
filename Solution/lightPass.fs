#version 440

out vec4 fragment_color;
out vec4 bright_color;

in vec2 texCoord0;

struct PointLight
{
	vec3 position;
	vec3 color;
};

uniform int NR_OF_POINT_LIGHTS;   

uniform vec3 cameraPos;
uniform PointLight PointLights[256];

// Gbuffer variables
uniform sampler2D gPosition;
uniform sampler2D gDiffuse;
uniform sampler2D gNormal;

// ShadowBuffer variables
uniform samplerCube shadowMap;
uniform float farPlane;

float calculateShadows(vec3 objPos, vec3 camPos, vec3 normal)
{
	vec3 lightToObj = objPos - PointLights[0].position.xyz;
    float viewDist = length(camPos - objPos);
	float currDepth = length(lightToObj);

    // Avoids "Shadow Acne"
    normal = normalize(normal);
	float bias = max(0.005 * (1 - dot(normal, lightToObj)), 0.005);
    
    // PCF (Percentage-closer filtering)
    // Change these two to optimize the look of the shadow
    int smoothness = 100; // Higher value will make the overall shadow "more compact"
    int samples = 30; // Higher value will lower the performance but make the smoothness' stages translate better

    float diskRadius = (1.0 + (viewDist / farPlane)) / smoothness; // Sharper the closer the camera is
    vec3 sampleOffsetDirections[20] = vec3[]
    (
        vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
        vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
        vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
        vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
        vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
    ); 

    // Calculate shadow
	float shadow = 0.0f;
    for (int i = 0; i < samples; i++)
    {
        float closestDepth = texture(shadowMap, lightToObj + sampleOffsetDirections[i] * diskRadius).r;
        closestDepth *= farPlane;
        if(currDepth - bias > closestDepth) 
        {
            shadow += 1.0f;
        }
    }
    shadow /= float(samples);

	return shadow;
}

void main()
{
	// Sample from the gBuffer
	vec3 pixelPos = texture2D(gPosition, texCoord0).xyz;
	vec3 materialColor = texture2D(gDiffuse, texCoord0).rgb;
	vec3 normal = texture2D(gNormal, texCoord0).xyz;

	// Attenuation
	float attenuation;
	float distancePixelToLight;

	// Ambient
	vec4 ambient = vec4(0.1f,0.1f,0.1,1.0f) * vec4(materialColor.rgb, 1.0f);
	
	// Diffuse
	vec3 lightDir;
	vec4 diffuse;
	float alpha;
	
	// Specular
	vec3 vecToCam;
	vec4 reflection;
	vec4 specular;
	float shininess = 30;

	for(int i = 0; i < NR_OF_POINT_LIGHTS; i++)
	{
		// Diffuse
		lightDir = normalize(PointLights[i].position.xyz - pixelPos.xyz);
		alpha = dot(normal.xyz,lightDir);
		diffuse += vec4(materialColor.rgb,1.0f) * vec4(PointLights[i].color.rgb, 1.0f) * max(alpha, 0);

		// Specular
		vecToCam = normalize(vec3(cameraPos.xyz - pixelPos.xyz));	
		// Source: https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/reflect.xhtml
		reflection = reflect(vec4(-lightDir.xyz, 0.0f), vec4(normal.xyz,1.0f));
		specular += vec4(materialColor.rgb,1.0f) * vec4(PointLights[i].color.rgb, 1.0f) * pow(max(dot(reflection.xyz, vecToCam.xyz),0), shininess);

		// attenuation
		distancePixelToLight = length(PointLights[i].position - pixelPos);
		attenuation = 1.0f / (1.0f + (0.1 * distancePixelToLight)+ (0.01 * pow(distancePixelToLight, 2)));
	}

	float shadow = calculateShadows(pixelPos, cameraPos, normal);

	vec4 finalColor = ambient + ((1 - shadow) * attenuation*(diffuse + specular));
	finalColor = min(vec4(1.0f,1.0f,1.0f,1.0f), finalColor);

	fragment_color = vec4(finalColor.xyz, 1.0f);

	// Calculate brightness (used for bloom)
	vec3 lumaVec = vec3(0.2126, 0.7152, 0.0722);
	float brightness = dot(fragment_color.rgb, lumaVec.xyz);

	if(brightness > 0.84f)
	{
		bright_color = vec4(fragment_color.rgb, 1.0f);
	}
	else
	{
		bright_color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	}
}