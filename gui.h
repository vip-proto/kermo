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

#if !defined(GUI_H)
#define GUI_H
#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Image.H>
#include <stdint.h>

struct Counters {
#define C_MAGIC 0x41414141
#define C_VERSION 1
   uint32_t magic;
   uint32_t size;
   uint32_t version;
   uint32_t starts;
   uint32_t time;
   void load(const char* filename);
   void save(const char* filename);
};

struct Additional_Data {
   Fl_Group* g;
   Counters c;
};

class Picture : public Fl_RGB_Image {
public:
   Picture(struct picture_data* picture);
   virtual ~Picture();
protected:
   struct picture_data* picture;
};

class Form : public Fl_Double_Window {
public:
   Form();
   virtual ~Form();
   void screen_up();
   void screen_down();
   virtual void set_position(int p);
   bool closing;
   const int& position;
protected:
   int p;
   virtual void activate(Fl_Group* o);
   virtual void deactivate(Fl_Group* o);
   virtual int key_down(int key);
   virtual int key_up(int key);
private:
   int handle(int event);
};

class ViP_Selector_Form : public Form {
public:
   ViP_Selector_Form();
   virtual ~ViP_Selector_Form();
   virtual void set_position(int p);
protected:
   int t, s;
   virtual int key_down(int key);
   virtual void refresh_pictures(struct entry* ig, bool delete_only = false);
   void scroll();
};

class Callidus_Cloud_Selector_Form : public Form {
public:
   Callidus_Cloud_Selector_Form();
   virtual ~Callidus_Cloud_Selector_Form();
   virtual void set_position(int p);
protected:
   int t, s;
   virtual int key_down(int key);
   virtual void refresh_pictures(struct entry* ig, bool delete_only = false);
   void scroll();
};
#endif
