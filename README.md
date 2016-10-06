To run: No command line arguments are needed.

LIGHTING ASSIGNMENT STUFF:
Pressing i increases the selected lighting variable, and pressing k decreases it. Pressing j changes the variable selected. The variables for lighting and material are printed to stdout, as well as the selected variable, which is bound to ltParam. Since ltParam is an int, its meaning is as follows:
0 - light ambiance
1 - light diffusion
2 - specular quality of light
3 - no longer bound
4 - material shininess
5 - material emission
6 - specular quality of material

EXTRA STUFF:
To toggle cube rotation, press 1. Arrow keys move, and page up and page down zoom, while in an orthographic view.

Press p to swap between orthographic and perspective projections. This is still working poorly, although I found a bug in my translation between last assignment and now.

w, a, s, and d work like video game controls: w steps forward, s backward, a and d strafe. These controls only work properly in perspective. I've limited the frame rate to 60 fps.

I had a stab at mouse-controlled movement since last assignment also. Hold the right mouse button and drag for some very interesting bugs, especially in orthographic view.

I spent about 12 hours on this project total.
