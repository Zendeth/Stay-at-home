#ifdef _MSC_VER // dummy for Visual Studio, which has no GTK
int gtkui(int argc, char* argv[]) {
  (void)argc;
  (void)argv;
  return 0;
}
#else
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#+
#+     Glade / Gtk Programming
#+
#+     Copyright (C) 2019 by Kevin C. O'Kane
#+
#+     Kevin C. O'Kane
#+     kc.okane@gmail.com
#+     https://www.cs.uni.edu/~okane
#+     http://threadsafebooks.com/
#+
#+ This program is free software; you can redistribute it and/or modify
#+ it under the terms of the GNU General Public License as published by
#+ the Free Software Foundation; either version 2 of the License, or
#+ (at your option) any later version.
#+
#+ This program is distributed in the hope that it will be useful,
#+ but WITHOUT ANY WARRANTY; without even the implied warranty of
#+ MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#+ GNU General Public License for more details.
#+
#+ You should have received a copy of the GNU General Public License
#+ along with this program; if not, write to the Free Software
#+ Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#+
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/


#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

#include "grsolve.h"

// Make them global

GtkWidget	*window;
GtkWidget	*fixed1;
GtkWidget	*button1;
GtkWidget	*button2;
GtkWidget	*label1;
GtkFileChooser	*file1;
GtkBuilder	*builder;
GtkWidget	*image1;
GtkWidget	*image2;
const char *filename;
GtkFileFilter *filter;
GtkWidget *event1;
int dw, dh;
int mystate = 0;  // 0 loading image, 1 recognition, 2 solve

p4_t clicks;  // points clicked
static int clickcnt = 0;

// initialize clicks to empty
void emptyclicks(void) {
  int i;
  for (i = 4; --i >= 0;)
    clicks.c[i].x = INT_MIN;
  clickcnt = 0;
}

// add a click
void addclick(int x, int y) {
  if (clickcnt == 4)
    for (clickcnt = 0; clickcnt < 4 - 1; ++clickcnt)
      clicks.c[clickcnt] = clicks.c[clickcnt + 1];
  clicks.c[clickcnt].x = x;
  clicks.c[clickcnt].y = y;
  ++clickcnt;
}

gboolean resize_image(GtkWidget* widget, GtkWidget* window) {
  GdkPixbuf* pb = gtk_image_get_pixbuf(GTK_IMAGE(widget));
  (void)window;
  if (pb == NULL)
  {
    g_printerr("Failed to resize image\n");
    return 1;
  }
  int width = gdk_pixbuf_get_width(pb);
  int height = gdk_pixbuf_get_height(pb);
  dw = 1200;
  dh = 800;
  if (width * (long)dh > height * (long)dw)
    dh = (long)dw * height / width; // reduce vertically
  else
    dw = (long)dh * width / height;
  pb = gdk_pixbuf_scale_simple(pb, dw, dh, GDK_INTERP_BILINEAR);

  gtk_image_set_from_pixbuf(GTK_IMAGE(widget), pb);
  gtk_container_add(GTK_CONTAINER(fixed1), image1);
  gtk_widget_show(image1);
  int hor = 720 - dw / 2;
  int ver = 450 - dh / 2;
  gtk_fixed_move(GTK_FIXED(fixed1), image1, hor, ver);
  gtk_fixed_move(GTK_FIXED(fixed1), event1, hor, ver);
  gtk_widget_set_size_request (event1,dw,dh);
  return FALSE;
}

static gboolean
button_press_callback (GtkWidget      *event_box,
                       GdkEventButton *event,
                       gpointer        data)
{
  (void)event_box;
  (void)data;

  if (0)
    g_print("Event box clicked at coordinates %f,%f\n",
      event->x, event->y);

  addclick(event->x, event->y);
    // Returning TRUE means we handled the event, so the signal
    // emission should be stopped (don’t call any further callbacks
    // that may be connected). Return FALSE to continue invoking callbacks.
    return TRUE;
}

// the GTK entry point
int gtkui(int argc, char* argv[]) {
  gtk_init(&argc, &argv); // init Gtk

//---------------------------------------------------------------------
// establish contact with xml code used to adjust widget settings
//---------------------------------------------------------------------

  builder = gtk_builder_new_from_file("gtkui.glade");

  window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));


  gtk_builder_connect_signals(builder, NULL);

  fixed1 = GTK_WIDGET(gtk_builder_get_object(builder, "fixed1"));
  button1 = GTK_WIDGET(gtk_builder_get_object(builder, "button1"));
  button2 = GTK_WIDGET(gtk_builder_get_object(builder, "button2"));
  label1 = GTK_WIDGET(gtk_builder_get_object(builder, "label1"));
  file1 = GTK_FILE_CHOOSER(gtk_builder_get_object(builder, "file1"));
  image1 = GTK_WIDGET(gtk_builder_get_object(builder, "image1"));
  filename = "numbers/frame_emptysudo.png";

  filter = gtk_file_filter_new();
  gtk_file_filter_add_pattern(filter, "*.jpg");
  gtk_file_filter_add_pattern(filter, "*.jpeg");
  gtk_file_filter_add_pattern(filter, "*.png");
  gtk_file_filter_add_pattern(filter, "*.gif");
  gtk_file_filter_add_pattern(filter, "*.bmp");
  gtk_file_filter_add_pattern(filter, "*.tif");
  gtk_file_filter_add_pattern(filter, "*.tiff");
  gtk_file_filter_add_pattern(filter, "*.pgm");
  gtk_file_chooser_add_filter(file1, filter);
  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

  event1 = GTK_WIDGET(gtk_builder_get_object(builder, "event1"));
  g_signal_connect(G_OBJECT(event1),
    "button_press_event",
    G_CALLBACK(button_press_callback),
    NULL);

  emptyclicks(); 
  gtk_container_remove(GTK_CONTAINER(event1), image1);
  resize_image(image1, window);

  gtk_widget_show_all(GTK_WIDGET(window));

  gtk_main();

  return EXIT_SUCCESS;
}

void	on_button1_clicked (GtkButton *b) {
  (void)b;
	gtk_label_set_text (GTK_LABEL(label1), (const gchar* ) "OCRed sudoku");
  if (filename == NULL)
    filename = "numbers/frame_emptysudo.png";
  filename = gtk_job(filename, &clicks, dw, dh);
  gtk_container_remove(GTK_CONTAINER(fixed1), image1); // remove old slide
  emptyclicks();
  image1 = gtk_image_new_from_file(filename);
  resize_image(image1, window);
  mystate = 2;
}

void	on_button2_clicked(GtkButton* b) {
  (void)b;
  if (mystate==1)
    on_button1_clicked(b);
  filename = gtk_solve();
  gtk_container_remove(GTK_CONTAINER(fixed1), image1); // remove old slide
  emptyclicks();
  image1 = gtk_image_new_from_file(filename);
  resize_image(image1, window);
}

void	on_file1_file_set(GtkFileChooserButton* f) {
  gtk_label_set_text(GTK_LABEL(label1), (const gchar*)"Load your sudoku and click on the four corners of the grid");
  filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(f));
  gtk_container_remove(GTK_CONTAINER(fixed1), image1); // remove old slide
  if (filename == NULL)
    filename = "numbers/frame_emptysudo.png";
  emptyclicks();
  image1 = gtk_image_new_from_file(filename);
  resize_image(image1, window);
  mystate = 1;
}

#endif // def _MSC_VER
