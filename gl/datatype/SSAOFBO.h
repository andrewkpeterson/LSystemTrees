#ifndef SSAOFBO_H
#define SSAOFBO_H


class SSAOFBO
{
public:
    SSAOFBO(int width, int height);
    ~SSAOFBO();
    void bindFBO();
    void unbindFBO();
    void bindSSAOTexture();


private:
    unsigned int fbo_handle;
    unsigned int ssao_color_buffer;
};

#endif // SSAOFBO_H
