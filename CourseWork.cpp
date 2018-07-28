#include "../../nclgl/window.h"
#include "Renderer.h"
#include <time.h>

#pragma comment(lib, "nclgl.lib")
int FPS = 0;
clock_t current_ticks, delta_ticks;
clock_t fps = 0;

int main() {	
	Window w("Index Buffers!", 1920,1080,true);
	if(!w.HasInitialised()) {
		return -1;
	}

	Renderer renderer(w);
	if(!renderer.HasInitialised()) {
		return -1;
	}

	w.LockMouseToWindow(true);
	w.ShowOSPointer(false);
	

	while(w.UpdateWindow() && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE)){

		current_ticks = clock();

		renderer.Input();
		renderer.UpdateScene(w.GetTimer()->GetTimedMS(), FPS);
		renderer.RenderScene();

		delta_ticks = clock() - current_ticks; 
		if (delta_ticks > 0)
			fps = CLOCKS_PER_SEC / delta_ticks;
		FPS = fps;
	}

	return 0;
}