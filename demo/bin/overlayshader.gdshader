shader_type spatial;
render_mode unshaded;
render_mode cull_disabled;
uniform sampler2D overlay;

void vertex() {
  POSITION = vec4(VERTEX, 1.0);
}

void fragment(){
	

  ALBEDO = texture(overlay, SCREEN_UV).xyz;
	//ALBEDO = vec3(UV,0);
  ALPHA = texture(overlay, SCREEN_UV).w;
}