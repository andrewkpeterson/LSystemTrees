#ifndef BLURFBO_H
#define BLURFBO_H


class BlurFBO
{
public:
    BlurFBO(int width, int height);
    ~BlurFBO();
    void bindFBO();
    void unbindFBO();
    void bindBlurredSSAOTexture();


private:
    unsigned int fbo_handle;
    unsigned int blur_texture;
};

#endif // BLURFBO_H
