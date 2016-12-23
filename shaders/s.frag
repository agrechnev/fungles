#version 300 es
precision mediump float;

in vec2 texCoord;
out vec4 fragColor;

// Use texture (true) or color (false) ?
uniform bool useTexture;

uniform sampler2D myTexture;
uniform vec4 myColor;

void main(){
   if (useTexture) {
     fragColor = texture(myTexture, texCoord);
   } else {
     fragColor=myColor;
   }
}
