#ifndef GBUFFER_H
#define GBUFFER_H


class GBuffer
{
public:
    GBuffer(int width, int height);
    ~GBuffer();

    void bindGBuffer();
    void unbindGBuffer();
    void bindPositionTexture();
    void bindNormalTexture();
    void bindDiffuseTexture();
    void bindSpecularTexture();


private:
    int m_width;
    int m_height;
    unsigned int fbo_handle;
    unsigned int position_tex;
    unsigned int normal_tex;
    unsigned int diffuse_tex;
    unsigned int specular_tex;


};

#endif // GBUFFER_H
