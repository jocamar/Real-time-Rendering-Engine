#Real-time Rendering - WS 2015 - Alley Demo

- João Marinheiro - 1528286
- Rui Neves - 1527958

##Effects implemented:

- Bloom - bloom was implemented using two-pass gaussian blur. It can be observed on the lights in the demo such as in the lamp.
- HDR - high-dynamic range was implemented using the Uncharted 2 tone-mapping operator with an exposure parameter. 
      The effects of HDR can be seen when the camera moves inside the container and when the helicopter moves close to the alley.
      By changing the exposure parameter, the image gets brighter/darker.
- Bump/Normal mapping - Normal mapping was implemented and can be seen in several places in the alley, including in non axis
                      aligned complex objects such as the droid.
- Directional/Omnidirectional shadows - Shadows were implemented both for directional and point lights using shadow maps. The
			   	      shadows are rendered onto a depth texture and used to test the fragments. Omni-directional
   				      shadows are achieved in 1 rendering pass instead of 6 by using the geometry shader to render
				      to different faces in the cubemap depending on the fragment direction.

##Demo description:
The demo shows a dark alley with a couple of objects. The camera and other objects are animated and there's no user control of the 
camera during the demo. Over the 1m20s duration all 4 implemented effects are shown, with the existance of 2 omnidirectional lights 
and a directional light as well as several models.

The user can change increase or decrease the gamma values for the scene by pressing the 'W' and 'S' keys. The user may also toggle 
the HDR rendering on/off by pressing the 'Q' key.

A video of the demo running can be seen on the following link: https://www.youtube.com/watch?v=nUKrgGvGLTk
