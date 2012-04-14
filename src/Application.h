#ifndef __APPLICATION__
#define __APPLICATION__

class Application {
private:
   SDL_Surface *videoSurface, *drawSurface;
   CairoDrawer *drawer;
   SDL_Thread *wiggleThread;
   const int mWidth;
   const int mHeight;

   jack_client_t *myClient;
   const char *clientName;
   vector<string> portNames;
   vector<jack_port_t *> outputPorts;
   jack_nframes_t mSampleRate;
   int channels;

   Spline spline1;
   Widget *rootWidget;
   Slider *slider1;
   TextBox *textbox1;
public:
   Application();

   vector<jack_port_t*> &getOutputPorts();
   void SetSampleRate(jack_nframes_t rate);
   void SetChannels(int c);

   int processBleeper(jack_nframes_t n, jack_default_audio_sample_t *out);
   int processPlunger(jack_nframes_t n, jack_default_audio_sample_t *out);
   int processSine(jack_nframes_t n, jack_default_audio_sample_t *out, double freq, double amplitude);
   int processSpline(jack_nframes_t n, jack_default_audio_sample_t *out,  double amplitude);

   void init_client();

   void shutdown_client();


   void finish();

   void sdl_fail();

   void handle_args(int argc, char *argv[]);

   SDL_Surface *setup_video_surface();

   SDL_Surface *setup_draw_surface();

   void test_drawing(Drawer &drawer);

   // Temporary
   void test_splines(Widget &rootWidget);
   void test_sliders(Widget &rootWidget);
   void test_textbox(Widget &rootWidget);

   // Sledgehammer ... repaint all
   // TODO replace with more efficient/elegant method
   void repaint();

   int wiggle_spline(void *);


   int init();
};

#endif
