#include <gdk-pixbuf/gdk-pixbuf.h>
#include <gtk/gtk.h>

GtkWidget		*window;
GtkWidget		*fixed1;
GtkWidget		*button_next;
GtkWidget		*button_back;

void next_fixed(GtkWidget *fixed)

int gui(int argc, char** argv)
{
	 gtk_init(&argc, &argv);

	builder = gtk_builder_new_from_file("gtkui.glade");

	window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));

	gtk_builder_connect_signals(builder, NULL);


	//Connects event handlers.
	g_signal_connect(button_next, "clicked", G_CALLBACK(next_fixed), NULL);
	g_signal_connect(button_back, "clicked", G_CALLBACK(previous_fixed), NULL);

	gtk_widget_show_all(GTK_WIDGET(window));

	 gtk_main();
	 return EXIT_SUCCESS;

}
