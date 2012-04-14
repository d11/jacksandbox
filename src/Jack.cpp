#include <jack/jack.h>
#include <iostream>
#include <string>
#include <exception>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <sstream>

#include <SDL.h>
#include <SDL_thread.h>

#include "Anim.h"
#include "Drawer.h"
#include "Skeleton.h"
#include "Spline.h"
#include "SplineView.h"
#include "Slider.h"
#include "Rect.h"
#include "TextBox.h"
#include "SplineNodeWiggler.h"

using namespace std;

#ifndef M_PI
#define M_PI   3.14159265358979323846
#endif
#define M_2PI  6.28318530717958647693

#include "Application.h"

Application *app;
bool running = true;

int process(jack_nframes_t numFrames, void *)
{
   //cout << pos << endl;
   /*vector<jack_port_t *>::iterator iter;
     for(iter=outputPorts.begin();iter!=outputPorts.end();++iter)
     {
     jack_default_audio_sample_t *out = (jack_default_audio_sample_t*)jack_port_get_buffer(*iter, numFrames);
   //processPlunger(numFrames/2, out);
   //processBleeper(numFrames/2, &out[numFrames/2]);
   //processBleeper(numFrames, out);
   processSine(numFrames, out);
   }*/
   jack_default_audio_sample_t *out = (jack_default_audio_sample_t*)jack_port_get_buffer(*app->getOutputPorts().begin(), numFrames);
   //processSine(numFrames, out, 440.0, 0.9);
   app->processSpline(numFrames, out,  0.9);
   return 0;
}

int srate(jack_nframes_t rate, void*)
{
   cout << "Sample rate change: " << rate << endl;
   app->SetSampleRate(rate);
   return 0;
}
void error (const char *desc)
{
   cerr << "JACK error: " + *desc << endl;
}

void jack_shutdown(void *)
{
   exit(1);
}

int xrun_callback(void *)
{
   cout << "XRUN" << endl;
   return 0;
}

Application::Application() : videoSurface(NULL), drawSurface(NULL), drawer(NULL),
   wiggleThread(NULL), mWidth(640), mHeight(480),
   myClient(NULL), clientName("jeremiah"), mSampleRate(48000),
   channels(2)
{}

vector<jack_port_t*> &Application::getOutputPorts() { return outputPorts; }
void Application::SetSampleRate(jack_nframes_t rate) { mSampleRate = rate; }
void Application::SetChannels(int c) { channels = c; }

int Application::processBleeper(jack_nframes_t n, jack_default_audio_sample_t *out)
{
   static double f = 50.0; // 'frequency' ... sort of
   static char b   = 0; // going up?
   cout << f << endl;
   jack_nframes_t k = 0; // index variable

   while (k < n) {
      out[k] = (double)(((double)(k*k)/(n*n))*sin(f*(double)k/n*M_PI*2));
      ++k;
   }
   if (b) f *= 1.01;
   else f *= 0.99;

   if (f > 180.0) b = 0;
   if (f < 10.0) b = 1;
   return 0;
}

int Application::processPlunger(jack_nframes_t n, jack_default_audio_sample_t *out)
{
   static double pos = 0.0;
   static double p = 0.10;
   static double dir = 0.9999;

   jack_nframes_t k = 0;
   while (k < n)
   {
      out[k] = pos;
      double move = (double)rand()/(double)RAND_MAX*dir*p;
      pos += move;
      if (pos > 1.0) {pos = 1.0; dir = dir *-0.999;}
      if (pos < -1.0){ pos = -1.0; dir = dir *-0.999;}
      ++k;
   }
   return 0;
}

int Application::processSine(jack_nframes_t n, jack_default_audio_sample_t *out, double freq, double amplitude)
{
   static double theta = 0.0;
   double dTheta = freq/(double)mSampleRate;
   jack_nframes_t k = 0;

   while (k < n)
   {
      out[k] = amplitude*sin(M_PI*2.0*theta);
      theta += dTheta;
      if (theta > 1.0) theta -= 1.0;
      ++k;
   }
   return 0;
}

int Application::processSpline(jack_nframes_t n, jack_default_audio_sample_t *out,  double amplitude)
{
   double freq = 110.0;
   if (slider1) freq += slider1->GetValue()*770.0;
   //if (!spline1) return 0;
   static double theta = 0.0;
   double dTheta = freq/(double)mSampleRate;
   jack_nframes_t k = 0;

   while (k < n)
   {
      out[k] = amplitude*spline1.GetValue(theta);
      //theta += dTheta; // 'straight'
      //theta += dTheta+0.0001*sin(SDL_GetTicks()*0.04); // 'vibrato'
      theta += dTheta+0.0001*sin(SDL_GetTicks()*0.05); // 'vibrato'
      //theta += dTheta+0.01*sin(SDL_GetTicks()*0.05); // 'laughing birds'
      //theta += dTheta+0.03*sin(SDL_GetTicks()*0.05);
      if (theta > 1.0) theta -= 1.0;
      ++k;
   }
   return 0;
}



void Application::init_client()
{
   jack_options_t options = JackNullOption;
   jack_status_t status;
   try {
      myClient = jack_client_open(clientName, options, &status);
      if (NULL == myClient)
         throw;
   } catch(...)
   {
      cout << "couldn't open client" << endl;
   }

   jack_set_process_callback(myClient, process, 0);
   int i;
   unsigned long bufSize = -1; //ignored

   for (i = 0; i < channels; ++i)
   {
      stringstream ss;
      ss << "output_" << i+1;
      portNames.push_back(ss.str());
      outputPorts.push_back(jack_port_register(myClient,
               portNames[i].c_str(),
               JACK_DEFAULT_AUDIO_TYPE,
               //JackPortIsOutput & JackPortIsTerminal,
               JackPortIsOutput,
               bufSize));
      cout << "created output port " << i << endl;
   }

   jack_set_sample_rate_callback (myClient, srate, 0);
   jack_set_error_function(error);
   jack_set_xrun_callback(myClient, xrun_callback, 0);
   jack_activate(myClient);
   if (int res = jack_connect(myClient, jack_port_name(outputPorts[0]), "system:playback_1"))
      cout << "couldn't connect" << res << endl;
}

void Application::shutdown_client()
{

   vector<jack_port_t*>::iterator iter;
   for (iter = outputPorts.begin(); iter != outputPorts.end(); ++iter)
   {
      jack_port_unregister(myClient, *iter);
   }

   try {
      if (0 != jack_deactivate(myClient)) throw;
   } catch (...) {
      cerr << "Faield to deactivate client." << endl;
   }
   try {
      if (0 != jack_client_close(myClient)) throw;
   } catch (...) {
      cerr << "Failed to close client." << endl;
   }
}



void Application::finish()
{
   delete drawer;
   delete rootWidget;
   SDL_Quit();
}

void Application::sdl_fail()
{
   cout << "[SDL failure] " << SDL_GetError() << endl;
   finish();
   abort();
}

void Application::handle_args(int argc, char *argv[])
{
   if (argc > 1)
      cout << "Args given:" << endl;
   int k = 1;
   while (k < argc) {
      cout << argv[k] << endl;
      ++k;
   }
}

SDL_Surface *Application::setup_video_surface()
{
   if (0 != SDL_Init(SDL_INIT_VIDEO))
      sdl_fail();

   SDL_Surface *videoSurface;
   videoSurface = SDL_SetVideoMode(mWidth, mHeight, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);

   if (NULL == videoSurface)
      sdl_fail();

   return videoSurface;
}

SDL_Surface *Application::setup_draw_surface()
{
   SDL_Surface *drawSurface;
   drawSurface = SDL_CreateRGBSurface (
         SDL_HWSURFACE, mWidth, mHeight, 32,
         0x00FF0000, /* Rmask */
         0x0000FF00, /* Gmask */
         0x000000FF, /* Bmask */
         0); /* Amask */
   return drawSurface;
}

void Application::test_drawing(Drawer &drawer)
{
   int i;
   double hGap = 20.0;
   double vHeight = 100.0;
   double vOffset = mHeight/2;
   double hOffset = 50.0;
   for (i = 0; i < 26; ++i){
      double x= hOffset+hGap*i;
      double y = vOffset + vHeight*sin(i/25.0*M_PI*2);
      Vector v1(x, y, 0);
      Vector v2(x+(13-i)*3, vOffset + vHeight, 0);
      char c = 65 + i;
      drawer.DrawLine(v1, v2);
      drawer.DrawText(v1, &c);
   }
}

// Temporary
void Application::test_splines(Widget &rootWidget)
{
   Spline &spl = spline1;
   spl.AddNode(Vector(0.0, 0.2, 0.0));
   spl.AddNode(Vector(0.2, 0.8, 0.0));
   spl.AddNode(Vector(0.5, 0.4, 0.0));
   spl.AddNode(Vector(0.8, 0.5, 0.0));
   spl.AddNode(Vector(1.0, 0.7, 0.0));

   Rect<double> rec;
   rec.h = 0.3;
   rec.w = 0.47;
   rec.x = 0.031;
   rec.y = 0.042;
   new SplineView(&spl, rec, &rootWidget);

   Rect<double> rec2;
   rec2.h = 0.47;
   rec2.w = 0.47;
   rec2.x = 0.5;
   rec2.y = 0.5;
   new SplineView(&spl, rec2, &rootWidget);
}

void Application::test_sliders(Widget &rootWidget)
{

   Rect<double> rec;
   rec.h = 0.08;
   rec.w = 0.3;
   rec.x = 0.1;
   rec.y = 0.8;
   slider1 = new Slider(rec, &rootWidget);
}

void Application::test_textbox(Widget &rootWidget)
{
   Rect<double> rec;
   rec.h = 0.08;
   rec.w = 0.4;
   rec.x = 0.1;
   rec.y = 0.6;
   textbox1 = new TextBox(rec, &rootWidget);

}

// Sledgehammer ... repaint all
// TODO replace with more efficient/elegant method
void Application::repaint()
{
   if (!drawSurface || !videoSurface) return;

   if (0 != SDL_LockSurface(drawSurface))
      sdl_fail();
   SDL_FillRect(drawSurface, &drawSurface->clip_rect, 0);
   drawer->BeginDrawing();
   rootWidget->Paint(*drawer);
   drawer->EndDrawing();
   SDL_UnlockSurface(drawSurface);
   if (0 != SDL_BlitSurface(drawSurface, NULL, videoSurface, NULL))
      sdl_fail();
   if (0 != SDL_Flip(videoSurface))
      sdl_fail();
}

int Application::wiggle_spline(void *)
{
   static long t  = 0;
   t = SDL_GetTicks();
   SplineNodeWiggler snvs;
   vector<double> freqs;
   freqs.push_back(2.0);
   freqs.push_back(1.4);
   freqs.push_back(1.3);
   freqs.push_back(0.9);
   freqs.push_back(2.0);
   snvs.SetFreqs(freqs);
   while (running)
   {
      t = SDL_GetTicks();
      snvs.SetTime(t*0.0002);
      spline1.VisitNodes(snvs);

      SDL_Delay(50);
   }
   return 0;
}

int render_loop(void *)
{
   while (running)
   {
      app->repaint();
      SDL_Delay(25); // draw at 40fps 
   }
   return 0;
}

int Application::init()
{

   init_client();

   // make wiggle thread
   //wiggleThread = SDL_CreateThread(wiggle_spline, NULL);
   wiggleThread = SDL_CreateThread(render_loop, NULL);
   if ( wiggleThread == NULL ) {
      cout << "Unable to create thread: " << SDL_GetError();
      return -1;
   }

   videoSurface = setup_video_surface();
   drawSurface = setup_draw_surface();


   drawer = new CairoDrawer(*drawSurface);
   rootWidget = new Widget("root", videoSurface->clip_rect, NULL);

   test_splines(*rootWidget);
   test_sliders(*rootWidget);
   test_textbox(*rootWidget);


   // Copy to the video memory
   if (0 != SDL_BlitSurface(drawSurface, NULL, videoSurface, NULL))
      sdl_fail();

   if (0 != SDL_Flip(videoSurface))
      sdl_fail();

   SDL_Event e;
   bool done = false;

   while((!done) && (SDL_WaitEvent(&e))) {
      switch(e.type) {
         case SDL_KEYUP:
            // Handle any key presses here.
            if (e.key.keysym.sym == SDLK_ESCAPE || e.key.keysym.sym == SDLK_q) {
               done = true;
               // TODO send to widgets
            }
            break;

         case SDL_MOUSEBUTTONDOWN:
            // TODO check which button
            // TODO focused widget
            rootWidget->OnClickDown(Vector(e.button.x, e.button.y));

            break;
         case SDL_MOUSEBUTTONUP:
            // TODO check which button
            rootWidget->OnClickUp(Vector(e.button.x, e.button.y));

            break;

         case SDL_MOUSEMOTION:
            rootWidget->OnMouseMove(Vector(e.motion.xrel, e.motion.yrel), Vector(e.motion.x, e.motion.y));
            /*printf("Mouse moved by %d,%d to (%d,%d)\n", 
              e.motion.xrel, e.motion.yrel,
              e.motion.x, e.motion.y);*/
            break;

         case SDL_QUIT:
            done = true;
            break;

         default:
            break;
      }
   }

   running = false;
   SDL_WaitThread(wiggleThread, NULL);

   finish();

   shutdown_client();

   return 0;
}

void read_parameters(int argc, char const *argv[])
{
   if (argc > 2)
   {
      cout << "Usage: ./Main" << endl;
   }
   int i;
   for (i = 0; i < argc; ++i)
   { }

   if (argc == 2)
   {
      cout << "yes" << endl;
      app->SetChannels(atoi(argv[1]));
   }
}

int main(int argc, char const* argv[])
{
   app = new Application();
   read_parameters(argc, argv);
   int res = app->init();

   delete app;
   return res;
}

