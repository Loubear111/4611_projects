# Assignment 5: Art Render


## Phong Shader
The Phong shader required the exact same calculations that were described by the professor in the Gouraud shader example. The only difference for the Phong shader is that we wanted to do these calculations per pixel rather than per vertex. This means we moved these equations to the ```phong.frag``` rather than the vert file since the frag file refers to each pixel (or fragment) of the model. The equations used to calculate ambient, diffuse, and specular lighting were as such:

```vec4 ambient = ka * Ia;```

```vec4 diffuse = ka * Id * max(dot(n, l), 0);```

```vec4 specular = ks * Is * pow(max(dot(h, n), 0), s);```

```
n = normalized normal of the pixel
l = vector from pixel to light source
h = halfway vector
```
## Artsy Shader
The artsy shader required almost exactly the same code as the Phong shader except that the diffuse and specular components were calculated using the texture files toonDiffuse.png and toonSpecular.png, respectively. To do this we map the diffuse and specular colors to the texture using the following equations:

```vec4 diffuse = ka * Id * texture(diffuse_ramp, texcoords);```

```vec4 specular = ks * Is * texture(specular_ramp, texcoords);```

**Note:** For the texcoords we simply fill in the "s" component with the specular intensity or diffuse intensity. (```texcoords.s = spec_intensity```)

### Artsy Outline
The outline was determined using the left and right normals as compared to the normal of the current vertex. Essentially if the dot product of the left normal to the eye normal and the right normal to the eye normal are opposite signs then we are working with a  vertex that should be an outline. The operation looks like this:

```
if(dot(to_eye, left_norm) * dot(to_eye, right_norm) < 0.0)
{
	// This vertex is on the outline!
}
```

To actually create the outline we  simply extend the vertex along its normal by a given thickness like so:

```vertex = vertex + (thickness*normal)```
