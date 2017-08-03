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
                                   1920, 1080, "The K E R M O - ViP PROTO Arcade cabinet game selector v2.1"),
   position(p) {
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
   closing = false;
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
      goto finish;

   switch (event) {
      case FL_KEYDOWN:
         ret = key_down(Fl::event_key());
         break;

      case FL_KEYUP:
         ret = key_up(Fl::event_key());
         break;
   }

   if (ret)
      goto finish;

   if (event == FL_KEYBOARD) {
      switch (Fl::event_key()) {
         case FL_Escape:
            ret = 1;
            closing = true;
            this->hide();
            break;
      }
   }

finish:
   return ret;
}

void Form::set_position(int p) {
   if (p >= n) p = n - 1;

   if (p < 0) p = 0;

   this->p = p;
}

ViP_Selector_Form::ViP_Selector_Form() : Form() {
   p = 0;
   t = 0;
   s = 10;

   for (int i = 0; i < n; i++) {
      Fl_Group* o = new Fl_Group(15, 340 + 70 * i, 1920 - 30, 60);
      ((Additional_Data*)buffer[i].user)->g = o;
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
      activate(((Additional_Data*)buffer[p].user)->g);
}

ViP_Selector_Form::~ViP_Selector_Form() {
   for (int i = 0; i < n; i++)
      refresh_pictures(&buffer[i], true);
}

int ViP_Selector_Form::key_down(int key) {
   if (n == 0)
      return 0;

   key &= 0xFF;
   int ret = 0;

   switch (key) {
      case 82:
         set_position(p - 1);
         ret = 1;
         break;

      case 84:
         set_position(p + 1);
         ret = 1;
         break;

      case 13:
      case 32:
         screen_down();
         ((Additional_Data*)buffer[p].user)->c.time += run_and_wait(&buffer[p], -89, 50,
               5);
         ((Additional_Data*)buffer[p].user)->c.starts++;
         ((Additional_Data*)buffer[p].user)->c.save(buffer[p].dir);
         refresh_pictures(&buffer[p]);

         if (!config.autoreload) screen_up();

         return 1;
   }

   return ret;
}

void ViP_Selector_Form::refresh_pictures(struct entry* ig, bool delete_only) {
   Fl_Group* g = ((Additional_Data*)ig->user)->g;
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
         ((Additional_Data*)buffer[i].user)->g->show();
         ((Additional_Data*)buffer[i].user)->g->position(15, 340 + 70 * (i - t));
      } else ((Additional_Data*)buffer[i].user)->g->hide();
}

void ViP_Selector_Form::set_position(int pp) {
   int l = p;
   Form::set_position(pp);

   if (p != l) {
      scroll();
      deactivate(((Additional_Data*)buffer[l].user)->g);
      activate(((Additional_Data*)buffer[p].user)->g);
      redraw();
   }
}

Callidus_Cloud_Selector_Form::Callidus_Cloud_Selector_Form() : Form() {
   p = 0;
   t = 0;
   s = 8;

   for (int i = 0; i < n; i++) {
      Fl_Group* o = new Fl_Group(15 + 475 * (i & 3), 320 + 380 * ((i >> 2) & 1), 465,
                                 370);
      ((Additional_Data*)buffer[i].user)->g = o;
      o->box(FL_FLAT_BOX);
      o->color(mk_color(config.entry_bg_color));
      o->labeltype(FL_NO_LABEL);
      {
         Fl_Box* o = new Fl_Box(15 + 475 * (i & 3), 640 + 380 * ((i >> 2) & 1), 465, 50,
                                buffer[i].title);
         o->labelfont(1);
         o->labelsize(32);
         o->align(Fl_Align(FL_ALIGN_CENTER | FL_ALIGN_INSIDE));
         o->labelcolor(mk_color(config.text_color));
      }
      {
         Fl_Box* o = new Fl_Box(25 + 475 * (i & 3), 330 + 380 * ((i >> 2) & 1), 445,
                                310);
         o->labelfont(1);
         o->labelsize(32);
         o->box(FL_NO_BOX);
         o->align(Fl_Align(FL_ALIGN_CENTER | FL_ALIGN_INSIDE));
      }

      if (buffer[i].multip) {
         Fl_Box* o = new Fl_Box(50 + 475 * (i & 3), 540 + 380 * ((i >> 2) & 1), 100,
                                100);
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
      activate(((Additional_Data*)buffer[p].user)->g);
}

Callidus_Cloud_Selector_Form::~Callidus_Cloud_Selector_Form() {
   for (int i = 0; i < n; i++)
      refresh_pictures(&buffer[i], true);
}

int Callidus_Cloud_Selector_Form::key_down(int key) {
   if (n == 0)
      return 0;

   key &= 0xFF;
   int ret = 0;

   switch (key) {
      case 81:
         set_position(p - 1);
         ret = 1;
         break;

      case 82:
         set_position(p - 4);
         ret = 1;
         break;

      case 83:
         set_position(p + 1);
         ret = 1;
         break;

      case 84:
         set_position(p + 4);
         ret = 1;
         break;

      case 13:
      case 32:
         screen_down();
         ((Additional_Data*)buffer[p].user)->c.time += run_and_wait(&buffer[p], -445,
               -310, 1);
         ((Additional_Data*)buffer[p].user)->c.starts++;
         ((Additional_Data*)buffer[p].user)->c.save(buffer[p].dir);
         refresh_pictures(&buffer[p]);

         if (!config.autoreload) screen_up();

         return 1;
   }

   return ret;
}

void Callidus_Cloud_Selector_Form::refresh_pictures(struct entry* ig,
      bool delete_only) {
   Fl_Group* g = ((Additional_Data*)ig->user)->g;
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
         ((Additional_Data*)buffer[i].user)->g->show();
         ((Additional_Data*)buffer[i].user)->g->position(15 + 475 * (i & 3),
               320 + 380 * (((i - t) >> 2) & 1));
      } else ((Additional_Data*)buffer[i].user)->g->hide();
}

void Callidus_Cloud_Selector_Form::set_position(int pp) {
   int l = p;
   Form::set_position(pp);

   if (p != l) {
      scroll();
      deactivate(((Additional_Data*)buffer[l].user)->g);
      activate(((Additional_Data*)buffer[p].user)->g);
      redraw();
   }
}

void Counters::load(const char* filename) {
   char s[32];
   snprintf(s, 32, "./%s.cnt", filename);
   FILE* f = fopen(s, "rb");
   magic = 0;

   if (f) {
      if (fread(this, 1, sizeof(Counters), f) != sizeof(Counters))
         magic = 0;

      fclose(f);
   }

   if (magic != C_MAGIC)
      memset(this, 0, sizeof(Counters));
}

void Counters::save(const char* filename) {
   char s[32];
   snprintf(s, 32, "./%s.cnt", filename);
   FILE* f = fopen(s, "wb");

   if (f) {
      magic = C_MAGIC;
      version = C_VERSION;
      fwrite(this, 1, sizeof(Counters), f);
      fclose(f);
   }
}

static int comparator(const void* p1, const void* p2) {
   entry* q1 = (entry*)p1;
   entry* q2 = (entry*)p2;
   Counters* c1 = &((Additional_Data*)(q1->user))->c;
   Counters* c2 = &((Additional_Data*)(q2->user))->c;
   int factor = 0;
   factor += (c1->time - c2->time) * config.time_sort_factor;
   factor += (c1->starts - c2->starts) * config.starts_sort_factor;
   factor += (int)(((long)p1 - (long)p2) / sizeof(void*)) *
             config.position_sort_factor;

   if (config.name_sort_factor)
      factor += strcmp(q1->title, q2->title) * config.name_sort_factor;

   return factor;
}

int main(int argc, char* argv[]) {
   Form *window = NULL;
   char dir[16] = { 0 };
   send_reset();
begin:
   parse_conf((argc >= 2) ? argv[1] : "mapper.conf");

   for (int i = 0; i < n; i++) {
      Additional_Data* a = new Additional_Data();
      a->c.load(buffer[i].dir);
      buffer[i].user = a;
   }

   if (config.name_sort_factor || config.time_sort_factor
         || config.time_sort_factor)
      qsort(buffer, n, sizeof(entry), comparator);

   switch (config.theme) {
      case THEME_CALLIDUSCLOUD:
         window = new Callidus_Cloud_Selector_Form();
         break;

      case THEME_VIPPROTO:
      default:
         window = new ViP_Selector_Form();
         break;
   };

   if (dir[0]) {
      for (int i = 0; i < n; i++)
         if (strcmp(dir, buffer[i].dir) == 0) {
            window->set_position(i);
            break;
         }
   }

#ifdef WIN32
   window->icon((char*)LoadIconA(GetModuleHandle(NULL), MAKEINTRESOURCEA(101)));
#endif
   Fl_Group::current(NULL);
   window->fullscreen();
   window->screen_up();
   Fl::run();

   if (config.autoreload && !window->closing) {
      strncpy(dir, buffer[window->position].dir, 16);
      goto begin;
   }

   delete window;

   for (int i = 0; i < n; i++)
      delete ((Additional_Data*)buffer[i].user);

   return 0;
}
