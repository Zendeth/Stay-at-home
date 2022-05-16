#include <gtk/gtk.h>
//#include <webkit2/webkit2.h>
#include <stdio.h>

GtkWidget		*window;
GtkWidget		*button_next;
GtkWidget		*button_back_qr;
GtkWidget		*button_start;
GtkWidget		*button_aboutus;
GtkWidget		*button_back_de;
GtkWidget		*button_next_de;
GtkWidget		*mainpage;
GtkWidget		*button_back_en;
GtkWidget		*button_next_en;
GtkWidget		*image;
GtkWidget		*qrcodepage;
GtkWidget		*decodepage;
GtkWidget		*encodepage;
GtkWidget		*url;
GtkBuilder		*builder;

// https://developer-old.gnome.org/gobject/stable/gobject-Signals.html#g-signal-connect

int main(int argc, char** argv)
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
	url = GTK_WIDGET(gtk_builder_get_object(builder, "url"));

	encodepage = GTK_WIDGET(gtk_builder_get_object(builder, "encodepage"));
	button_next_en = GTK_WIDGET(gtk_builder_get_object(builder, "button_next_en"));
	button_back_en = GTK_WIDGET(gtk_builder_get_object(builder, "button_back_en"));
	image = GTK_WIDGET(gtk_builder_get_object(builder, "image"));
	

	//Connects event handlers.
	//g_signal_connect(button_next, "clicked", G_CALLBACK(next_fixed), NULL);
	//g_signal_connect(button_back, "clicked", G_CALLBACK(previous_fixed), NULL);
	/*
	g_signal_connect(button_aboutus, "clicked", G_CALLBACK(gtk_widget_hide), &mainpage);
	g_signal_connect(button_start, "clicked", G_CALLBACK(on_button_next_clicked), &mainpage);
	g_signal_connect(button_back_qr, "clicked", G_CALLBACK(on_button_back_clicked), &qrcodepage);
	g_signal_connect(button_back_de, "clicked", G_CALLBACK(on_button_back_clicked), &decodepage);
	g_signal_connect(button_back_en, "clicked", G_CALLBACK(on_button_back_clicked), &encodepage);
	g_signal_connect(button_next_de, "clicked", G_CALLBACK(on_button_next_de_clicked), &qrcodepage);
	g_signal_connect(button_next_en, "clicked", G_CALLBACK(on_button_next_en_clicked), &qrcodepage);
	*/

	gtk_widget_show(mainpage);

	gtk_main();
	
	return EXIT_SUCCESS;

}

/*
void on_button_next_de_clicked(GtkWidget *before)
{
	gtk_widget_hide(before);
	GtkWidget *after = GTK_WIDGET(gtk_builder_get_object(builder, "button_next_de"));
	gtk_widget_show(after);
	//url = "";
	// url is the output 
	GtkTextBuffer *buf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(url));
	gtk_text_buffer_set_text(buf, url);
	
}

void on_button_next_en_clicked(GtkWidget *before)
{
	gtk_widget_hide(before);
	GtkWidget *after = GTK_WIDGET(gtk_builder_get_object(builder, "button_next_en"));
	gtk_widget_show(after);
	// gtk_image_set_from_file(image, path);
}

void on_button_back_clicked(GtkWidget *before)
{
	gtk_widget_hide(before);
	//gtk_widget_show(after);
}*/