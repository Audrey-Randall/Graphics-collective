Flooded Ruins: Audrey Randall

To run: make clean; make; ./main

WHY THIS PROJECT IS WORTH AN A:

There are two elements of this project I consider worthy of an A. First, I'm using shaders to render lighting (and everything else), which includes generating normals from textures pixel by pixel. This involved writing my own lighting calculations, which gave me an excellent understanding of how they work. I also figured out how to create waterlike distortion by altering texture coordinates and a waterlike normal map by juxtaposing two sliding normal maps, using shaders. But I have to admit I think the light is still bugged. I used five separate shaders in this project.
	Second, in order to render objects under water as distorted and pave the way to add reflections later, I figured out how to set up a frame buffer, use it to render a scene to a texture, and draw said texture to a quad the size of the screen so that a separate shader could be applied to distort it. This involved some tricks with the depth buffer, to ensure that the plane on which underwater objects are drawn is always in front of the skybox and behind everything else. 

TO VIEW:

I'm not proud of the movement controls :P the scene really bugs out with mine, so I just copied yours, and the projection is wrong. I suggest zooming out with the page_down key and viewing the tower from all angles, the stairs are a bit confusing with the distortion and the texture. I don't know how to generate a normal map so I had to use a poorly-fitting stone texture, it was all I could find.

KNOWN BUGS:

Lighting is still not accurate! 

Don't even try to use perspective projections or first person movement....

Depth fade is based on window space not world space.


EXTRA STUFF:
Arrow keys move, and page up and page down zoom, while in an orthographic view.

I'm estimating 60 hours on this project.
