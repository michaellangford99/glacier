#include "framebuffer.h"

framebuffer::framebuffer(unsigned int width, unsigned int height, std::shared_ptr<texture> tex) : width(width), height(height), fb_texture(tex)
{
    glGenFramebuffers(1, &ID_framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, ID_framebuffer);

    //attach texture attachment
    //TODO: arbitrary texture attachments
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fb_texture->gl_texture_id, 0);

	std::cout << __FILE__ << ":" << __LINE__ << " error status: " << glGetError() << std::endl;
    //create depth and stencil attachment
    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo); 
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);  
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

	std::cout << __FILE__ << ":" << __LINE__ << " error status: " << glGetError() << std::endl;
    
    //attach it
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) std::cout << "ERROR! couldn't create fb" << std::endl;
    
    //creation complete, unbind
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    std::cout << __FILE__ << ":" << __LINE__ << " error status: " << glGetError() << std::endl;

}

//todo: resize functions

void framebuffer::bind_and_predraw()
{
     //bind for draw stage
    glBindFramebuffer(GL_FRAMEBUFFER, ID_framebuffer);
    
    glViewport(0,0,width,height);

    std::cout << "w: " << width << " h: " << height << std::endl;
    
    glClearColor(0, 1, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    //todo save settings with fb
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);
}

void framebuffer::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

framebuffer::~framebuffer()
{
    //glDeleteFramebuffers(1, &ID_framebuffer);  
}
