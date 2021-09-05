- Common  
The commonly used skybox shader, we can directly specifies the cubemap
- Equirectangular2Cubemap  
It uses equirectangular HDR texture to generate environment cube map for PBR shaders
- Irradiance_Convolution  
It uses environment cube map to generate IrradianceMap for PBR shaders
- PrefilterMap  
It uses environment cube map to generate a Mipmap for PBR shaders
- Cubemap  
It can be combined with different fragment shaders : Equirectangular2Cubemap, Irradiance_Convolution, PrefilterMap
- Equirectangular  
Using an equirectangular texture to render skybox