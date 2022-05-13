#include <gtk/gtk.h>

GtkWidget		*window;
GtkWidget		*button_next;
GtkWidget		*button_back;
GtkWidget		*button_aboutus;
GtkWidget		*mainpage;
GtkWidget		*qrcodepage;
GtkWidget		*decodepage;
GtkWidget		*encodepage;
GtkBuilder		*builder;

// https://developer-old.gnome.org/gobject/stable/gobject-Signals.html#g-signal-connect

int gui(int argc, char** argv)
{
	gtk_init(&argc, &argv);

	builder = gtk_builder_new_from_file("GUI.glade");

	gtk_builder_connect_signals(builder, NULL);
	
	mainpage = GTK_WIDGET(gtk_builder_get_object(builder, "mainpage"));
	button_aboutus = GTK_WIDGET(gtk_builder_get_object(builder, "button_aboutus"));
	button_start = GTK_WIDGET(gtk_builder_get_object(builder, "button_start"));

	qrcodepage = GTK_WIDGET(gtk_builder_get_object(builder, "qrcodepage"));
	button_back_qr = GTK_WIDGET(gtk_builder_get_object(builder, "button_back_qr"));

	decodepage = GTK_WIDGET(gtk_builder_get_object(builder, "decodepage"));
	button_next_de = GTK_WIDGET(gtk_builder_get_object(builder, "button_next_de"));
	button_back_de = GTK_WIDGET(gtk_builder_get_object(builder, "button_back_de"));

	encodepage = GTK_WIDGET(gtk_builder_get_object(builder, "encodepage"));
	button_next_en = GTK_WIDGET(gtk_builder_get_object(builder, "button_next_en"));
	button_back_en = GTK_WIDGET(gtk_builder_get_object(builder, "button_back_en"));


	//Connects event handlers.
	//g_signal_connect(button_next, "clicked", G_CALLBACK(next_fixed), NULL);
	//g_signal_connect(button_back, "clicked", G_CALLBACK(previous_fixed), NULL);
	g_signal_connect_swapped(button_aboutus, "clicked", G_CALLBACK(gtk_widget_hide), &mainpage);

	g_signal_connect_swapped(button_start, "clicked", G_CALLBACK(gtk_widget_show_all(GTK_WIDGET(encode_window))), &mainpage);

	gtk_widget_show_all(GTK_WIDGET(window));

	gtk_main();
	
	return EXIT_SUCCESS;

}
