/******************************************************************************
  This file is part of Arcade cabinet project.

  Copyright (C) 2014 ViP-PROTO Association, http://vip-proto.com
  Aleksandar Rikalo <aleksandar.rikalo@vip-proto.rs>

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License as
  published by the Free Software Foundation; either version 2 of the
  License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
  02111-1307, USA.

  The GNU General Public License is contained in the file COPYING.
******************************************************************************/

#include <stdio.h>

#ifdef WIN32
   #include <windows.h>
   #include <FL/x.h>
#else
   #include <unistd.h>
#endif

#include "kermo.h"
#include "selektor.h"
#include "gui.h"

#define mk_color(a) ( a ? ((Fl_Color)(a << 8)) : FL_BLACK)

Picture::Picture(struct picture_data* p)
        : Fl_RGB_Image(p->data, p->width, p->height, p->bytes_per_pixel) {
   this->picture = p;
}

Picture::~Picture() {
   free(picture);
}

Form::Form() : Fl_Double_Window(0, 0,
         1920, 1080, "The K E R M O - ViP PROTO Arcade cabinet game selector v2.0") {
   this->box(FL_FLAT_BOX);
   this->color(mk_color(config.bg_color));
   this->selection_color(mk_color(config.bg_color));
   this->labeltype(FL_NO_LABEL);
   this->labelfont(0);
   this->labelsize(14);
   this->align(Fl_Align(FL_ALIGN_TOP));
   this->when(FL_WHEN_RELEASE);
   Fl_Box* o = new Fl_Box(0, 0, 1920, 315);
   o->align(Fl_Align(FL_ALIGN_CENTER | FL_ALIGN_INSIDE));
   o->color(mk_color(config.bg_color));
   o->image(&image_logoBW_hor_1920);
   if (config.jpeg_logo_file[0]) {
      struct picture_data *pd = read_and_resize_jpeg(config.jpeg_logo_file, 0, 0);
      if (pd) o->image(new Picture(pd));
   }
   this->set_modal();
   this->clear_border();
}

Form::~Form() {
   if (this->child(0)->image() != &image_logoBW_hor_1920)
      delete this->child(0)->image();
}

void Form::screen_up() {
   show();
   cursor(FL_CURSOR_NONE);
}

void Form::screen_down() {
   hide();
   cursor(FL_CURSOR_DEFAULT);
   Fl::flush();
}

void Form::activate(Fl_Group* o) {
   Fl_Box* title = (Fl_Box*)o->child(0);
   o->color(mk_color(config.selected_entry_bg_color));
   title->labelcolor(mk_color(config.selected_text_color));
}

void Form::deactivate(Fl_Group* o) {
   Fl_Box* title = (Fl_Box*)o->child(0);
   o->color(mk_color(config.entry_bg_color));
   title->labelcolor(mk_color(config.text_color));
}

int Form::key_down(int key) {
   return 0;
}

int Form::key_up(int key) {
   return 0;
}

int Form::handle(int event) {
   int ret = Fl_Double_Window::handle(event);

   if (ret)
      return ret;

   switch (event) {
      case FL_KEYDOWN:
         return key_down(Fl::event_key());
      case FL_KEYUP:
         return key_up(Fl::event_key());
   }

   return 0;
}

ViP_Selector_Form::ViP_Selector_Form() : Form() {
   p = 0;
   t = 0;
   s = 10;
   for (int i = 0; i < n; i++) {
      Fl_Group* o = new Fl_Group(15, 340 + 70 * i, 1920 - 30, 60);
      buffer[i].user = o;
      o->box(FL_FLAT_BOX);
      o->color(mk_color(config.entry_bg_color));
      o->labeltype(FL_NO_LABEL);
      {
         Fl_Box* o = new Fl_Box(530, 350 + 70 * i, 1390, 40, buffer[i].title);
         o->labelfont(1);
         o->labelsize(32);
         o->align(Fl_Align(FL_ALIGN_TOP_LEFT | FL_ALIGN_INSIDE));
         o->labelcolor(mk_color(config.text_color));
      }
      for (int j = 0; j < 5; j++) {
         Fl_Box* o = new Fl_Box(-3 + 25 + 99 * j, 345 + 70 * i, 89, 50);
         o->labelfont(1);
         o->labelsize(32);
         o->box(FL_NO_BOX);
         o->align(Fl_Align(FL_ALIGN_TOP_LEFT | FL_ALIGN_INSIDE));
      }

      if (i >= t + s) o->hide();

      refresh_pictures(&buffer[i]);

      o->end();
   }
   if (n)
      activate((Fl_Group*)buffer[p].user);
}

ViP_Selector_Form::~ViP_Selector_Form() {
   for (int i = 0; i < n; i++)
      refresh_pictures(&buffer[i], true);
}

int ViP_Selector_Form::key_down(int key) {
   if (n == 0)
      return 0;

   key &= 0xFF;
   int l = p, ret = 0;

   switch (key) {
      case 82:
         p--;

         if (p < 0) p = 0;

         ret = 1;
         break;

      case 84:
         p++;

         if (p >= n) p = n - 1;

         ret = 1;
         break;

      case 13:
      case 32:
         screen_down();
         run_and_wait(&buffer[p], -89, 50, 5);
         refresh_pictures(&buffer[p]);
         screen_up();
         return 1;
   }

   if (p != l) {
      scroll();
      deactivate((Fl_Group*)buffer[l].user);
      activate((Fl_Group*)buffer[p].user);
      redraw();
   }

   return ret;
}

void ViP_Selector_Form::refresh_pictures(struct entry* ig, bool delete_only) {
   Fl_Group* g = (Fl_Group*)ig->user;
   char s[128];
   struct picture_data* picture;
   for (int i = 0; i < 5; i++) {
      Fl_Box* o = (Fl_Box*)g->child(i + 1);
      delete o->image();
      o->image(NULL);
      if (delete_only)
         continue;
      sprintf(s, "./%s_%u.jpg", ig->dir, i);
      picture = read_and_resize_jpeg(s, 0, 50);
      if (picture) {
         o->image(new Picture(picture));
      }
   }
}

void ViP_Selector_Form::scroll() {
   if (p >= t && p < t + s) return;

   if (p < t) t = p;

   if (p >= t + s) t = p - s + 1;

   for (int i = 0; i < n; i++)
      if (i >= t && i < t + s) {
         ((Fl_Group*)buffer[i].user)->show();
         ((Fl_Group*)buffer[i].user)->position(15, 340 + 70 * (i - t));
      } else ((Fl_Group*)buffer[i].user)->hide();
}

Callidus_Cloud_Selector_Form::Callidus_Cloud_Selector_Form() : Form() {
   p = 0;
   t = 0;
   s = 8;
   for (int i = 0; i < n; i++) {
      Fl_Group* o = new Fl_Group(15 + 475 * (i & 3), 320 + 380 * ((i >> 2) & 1), 465, 370);
      buffer[i].user = o;
      o->box(FL_FLAT_BOX);
      o->color(mk_color(config.entry_bg_color));
      o->labeltype(FL_NO_LABEL);
      {
         Fl_Box* o = new Fl_Box(15 + 475 * (i & 3), 640 + 380 * ((i >> 2) & 1), 465, 50, buffer[i].title);
         o->labelfont(1);
         o->labelsize(32);
         o->align(Fl_Align(FL_ALIGN_CENTER | FL_ALIGN_INSIDE));
         o->labelcolor(mk_color(config.text_color));
      }
      {
         Fl_Box* o = new Fl_Box(25 + 475 * (i & 3), 330 + 380 * ((i >> 2) & 1), 445, 310);
         o->labelfont(1);
         o->labelsize(32);
         o->box(FL_NO_BOX);
         o->align(Fl_Align(FL_ALIGN_CENTER | FL_ALIGN_INSIDE));
      }
      if (buffer[i].multip) {
         Fl_Box* o = new Fl_Box(50 + 475 * (i & 3), 540 + 380 * ((i >> 2) & 1), 100, 100);
         o->labelfont(1);
         o->labelsize(32);
         o->box(FL_NO_BOX);
         o->align(Fl_Align(FL_ALIGN_TOP_LEFT | FL_ALIGN_INSIDE));
         o->image(image_2p);
      }

      if (i >= t + s) o->hide();

      refresh_pictures(&buffer[i]);

      o->end();
   }
   if (n)
      activate((Fl_Group*)buffer[p].user);
}

Callidus_Cloud_Selector_Form::~Callidus_Cloud_Selector_Form() {
   for (int i = 0; i < n; i++)
      refresh_pictures(&buffer[i], true);
}

int Callidus_Cloud_Selector_Form::key_down(int key) {
   if (n == 0)
      return 0;

   key &= 0xFF;
   int l = p, ret = 0;

   switch (key) {
      case 81:
         p--;

         if (p < 0) p = 0;

         ret = 1;
         break;

      case 82:
         p -= 4;

         if (p < 0) p = 0;

         ret = 1;
         break;

      case 83:
         p++;

         if (p >= n) p = n - 1;

         ret = 1;
         break;

      case 84:
         p += 4;

         if (p >= n) p = n - 1;

         ret = 1;
         break;

      case 13:
      case 32:
         screen_down();
         run_and_wait(&buffer[p], -445, -310, 1);
         refresh_pictures(&buffer[p]);
         screen_up();
         return 1;
   }

   if (p != l) {
      scroll();
      deactivate((Fl_Group*)buffer[l].user);
      activate((Fl_Group*)buffer[p].user);
      redraw();
   }

   return ret;
}

void Callidus_Cloud_Selector_Form::refresh_pictures(struct entry* ig, bool delete_only) {
   Fl_Group* g = (Fl_Group*)ig->user;
   char s[128];
   struct picture_data* picture;
   Fl_Box* o = (Fl_Box*)g->child(1);
   delete o->image();
   o->image(NULL);
   if (delete_only)
      return;
   sprintf(s, "./%s_0.jpg", ig->dir);
   picture = read_and_resize_jpeg(s, 0, 0);
   if (picture) {
      o->image(new Picture(picture));
   }
}

void Callidus_Cloud_Selector_Form::scroll() {
   if (p >= t && p < t + s) return;

   if (p < t) t = p & ~3;

   if (p >= t + s) t = (p - s + 4) & ~3;

   for (int i = 0; i < n; i++)
      if (i >= t && i < t + s) {
         ((Fl_Group*)buffer[i].user)->show();
         ((Fl_Group*)buffer[i].user)->position(15 + 475 * (i & 3), 320 + 380 * (((i - t) >> 2) & 1));
      } else ((Fl_Group*)buffer[i].user)->hide();
}

int main(int argc, char* argv[]) {
   Form *window = NULL;
   FILE *f = NULL;

   if (argc < 2)
      f = fopen("mapper.conf", "rt");
   else
      f = fopen(argv[1], "rt");

   if (f) {
      yyin = f;
      yyparse();
      fclose(f);
   }

   send_reset();

   switch (config.theme) {
      case THEME_CALLIDUSCLOUD:
         window = new Callidus_Cloud_Selector_Form();
      break;

      case THEME_VIPPROTO:
      default:
         window = new ViP_Selector_Form();
      break;
   };

#ifdef WIN32
   window->icon((char*)LoadIconA(GetModuleHandle(NULL), MAKEINTRESOURCEA(101)));
#endif
   Fl_Group::current(NULL);
   window->fullscreen();
   window->screen_up();
   Fl::run();
   delete window;
   return 0;
}
