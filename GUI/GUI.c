#include <gtk/gtk.h>

GtkWidget		*window;
GtkWidget		*button_next;
GtkWidget		*button_back;
GtkWidget		*button_aboutus;
GtkWidget		*mainpage;
GtkWidget		*qrcodepage;
GtkBuilder		*builder;

int gui(int argc, char** argv)
{
	gtk_init(&argc, &argv);

	builder = gtk_builder_new_from_file("gui.glade");

	window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));

	gtk_builder_connect_signals(builder, NULL);

	qrcodepage = GTK_WIDGET(gtk_builder_get_object(builder, "qrcodepage"));
	button_next = GTK_WIDGET(gtk_builder_get_object(builder, "button_next"));
	button_back = GTK_WIDGET(gtk_builder_get_object(builder, "button_back"));
	button_aboutus = GTK_WIDGET(gtk_builder_get_object(builder, "button_aboutus"));
	mainpage = GTK_WIDGET(gtk_builder_get_object(builder, "mainpage"));
	qrcodepage = GTK_WIDGET(gtk_builder_get_object(builder, "qrcodepage"));


	//Connects event handlers.
	//g_signal_connect(button_next, "clicked", G_CALLBACK(next_fixed), NULL);
	//g_signal_connect(button_back, "clicked", G_CALLBACK(previous_fixed), NULL);
	g_signal_connect_swapped(button_aboutus, "clicked", G_CALLBACK(gtk_widget_hide), &mainpage);

	gtk_widget_show_all(GTK_WIDGET(window));

	gtk_main();
	return EXIT_SUCCESS;
}
