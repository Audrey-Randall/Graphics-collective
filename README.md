To run: make, then ./main or main.exe depending on your OS

This project is going to culminate in (hopefully) realistic-looking water. I have a cheating version of a depth fade working, as well as sliding normal maps, implemented with shaders. I still want diffraction ands reflection before next week, and I'll fix the depth fog if I have time (it's based on window coordinates nor world coordinates at the moment).

Press space to start and stop light rotation. See Extra Stuff for movement instructions.

KNOWN BUGS:

Lighting is still not accurate, although I fixed my half-vector calculation between the last time you saw this and now. You can see that the lighting disappears on the plane when the light is at certain angles. The teapots appear to show it correctly, though.

Switching from perspective to orthographic modes doesn't work well, there's a bug somewhere in my translation. Also, don't use the movement controls for the wrong perspective! It breaks everything and you'll have to restart the program.

Depth fade is based on window space not world space.

TODO:

Diffraction and reflections

Find out why transparency doesn't work! I'm writing alpha values, GL_BLEND is enabled, the blend modes are set, what am I missing?

Create a flooded room with stairs instead of a stack of teapots with a hat, once everything works :D

Bug fixes

EXTRA STUFF:
Arrow keys move, and page up and page down zoom, while in an orthographic view.

Press p to swap between orthographic and perspective projections. This is STILL working poorly.

In perspective, w, a, s, and d work like video game controls: w steps forward, s backward, a and d strafe.

So far, I'm estimating 40 hours on this project.
