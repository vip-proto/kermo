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
#include <FL/images/jpeglib.h>
/* Original libjpeg also can be used. */
#include <malloc.h>
#include <setupapi.h>
#include <ddk/Hidsdi.h>

#else
#include <unistd.h>
#include <sys/wait.h>
#include <jpeglib.h>
#include <X11/Xlib.h>
#include <X11/X.h>
#include <X11/Xutil.h>
#include <errno.h>
#endif

#include <sys/stat.h>
#include <math.h>
#include <setjmp.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "kermo.h"

#define MIN(A,B) ((A > B) ? (B) : (A))

static struct full_map def_map = {
   .p1 = {
      KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT,
      KEY_ENTER, KEY_ENTER, KEY_ENTER, KEY_ENTER,
      KEY_ENTER, KEY_ENTER, KEY_ENTER, KEY_ENTER,
      KEY_NONE, KEY_NONE, KEY_NONE, KEY_NONE,

      KEY_NONE, KEY_NONE, KEY_NONE, KEY_NONE,
      KEY_NONE, KEY_NONE, KEY_NONE, KEY_NONE,
      KEY_NONE, KEY_NONE, KEY_NONE, KEY_NONE,
      KEY_NONE, KEY_NONE, KEY_NONE, KEY_NONE,

      KEY_NONE, KEY_NONE, KEY_NONE, KEY_NONE,
      KEY_NONE, KEY_NONE, KEY_NONE, KEY_NONE,
      KEY_NONE, KEY_NONE, KEY_NONE, KEY_NONE,
      KEY_NONE, KEY_NONE, KEY_NONE, KEY_NONE
   },

   .p2 = {
      KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT,
      KEY_ENTER, KEY_ENTER, KEY_ENTER, KEY_ENTER,
      KEY_ENTER, KEY_ENTER, KEY_ENTER, KEY_ENTER,
      KEY_NONE, KEY_NONE, KEY_NONE, KEY_NONE,

      KEY_NONE, KEY_NONE, KEY_NONE, KEY_NONE,
      KEY_NONE, KEY_NONE, KEY_NONE, KEY_NONE,
      KEY_NONE, KEY_NONE, KEY_NONE, KEY_NONE,
      KEY_NONE, KEY_NONE, KEY_NONE, KEY_NONE,

      KEY_NONE, KEY_NONE, KEY_NONE, KEY_NONE,
      KEY_NONE, KEY_NONE, KEY_NONE, KEY_NONE,
      KEY_NONE, KEY_NONE, KEY_NONE, KEY_NONE,
      KEY_NONE, KEY_NONE, KEY_NONE, KEY_NONE
   }
};

void send_reset() {
   send_map(&def_map);
}

static char *str_replace (const char *string, const char *substr,
                          const char *replacement) {
   char *tok = NULL;
   char *newstr = NULL;
   char *oldstr = NULL;
   char *head = NULL;

   if (substr == NULL || replacement == NULL)
      return strdup (string);

   newstr = strdup (string);
   head = newstr;

   while ((tok = strstr (head, substr))) {
      oldstr = newstr;
      newstr = malloc ( strlen ( oldstr ) - strlen ( substr ) + strlen (
                           replacement ) + 1 );

      if (newstr == NULL) {
         free (oldstr);
         return NULL;
      }

      memcpy (newstr, oldstr, tok - oldstr);
      memcpy (newstr + (tok - oldstr), replacement, strlen(replacement));
      memcpy (newstr + (tok - oldstr) + strlen(replacement), tok + strlen(substr),
              strlen(oldstr) - strlen(substr) - (tok - oldstr));
      memset (newstr + strlen(oldstr) - strlen(substr) + strlen (replacement) , 0, 1);
      head = newstr + (tok - oldstr) + strlen(replacement);
      free (oldstr);
   }

   return newstr;
}

static void resize_image(void* src, int src_width, int src_height,
                         void* dst, int dst_width, int dst_height,
                         int ignore_alpha) {
   static int* g_px1a    = NULL;
   static int  g_px1a_w  = 0;
   static int* g_px1ab   = NULL;
   static int  g_px1ab_w = 0;
   unsigned int *dsrc  = (unsigned int *)src;
   unsigned int *ddest = (unsigned int *)dst;
   int bUpsampleX = (src_width < dst_width);
   int bUpsampleY = (src_height < dst_height);
   int weight_shift = 0;
   float source_texels_per_out_pixel = ((src_width / (float)dst_width + 1) *
                                        (src_height / (float)dst_height + 1));
   float weight_per_pixel = source_texels_per_out_pixel * 256 * 256;
   float accum_per_pixel = weight_per_pixel * 256;
   float weight_div = accum_per_pixel / 4294967000.0f;

   if (weight_div > 1)
      weight_shift = (int)ceilf(logf((float)weight_div) / logf(2.0f));

   weight_shift = MIN(15, weight_shift);
   float fh = 256 * src_height / (float)dst_height;
   float fw = 256 * src_width / (float)dst_width;

   if (bUpsampleX && bUpsampleY) {
      if (g_px1a_w < dst_width) {
         if (g_px1a) free(g_px1a);

         g_px1a = (int*)malloc(4 * dst_width * 2 * 1);
         g_px1a_w = dst_width * 2;
      }

      int x2;

      for (x2 = 0; x2 < dst_width; x2++) {
         int x1a = (int)(x2 * fw);
         x1a = MIN(x1a, 256 * (src_width - 1) - 1);
         g_px1a[x2] = x1a;
      }

      int y2;

      for (y2 = 0; y2 < dst_height; y2++) {
         int y1a = (int)(y2 * fh);
         y1a = MIN(y1a, 256 * (src_height - 1) - 1);
         int y1c = y1a >> 8;
         unsigned int *ddest = &((unsigned int *)dst)[y2 * dst_width];
         int x2;

         for (x2 = 0; x2 < dst_width; x2++) {
            int x1a = g_px1a[x2];//(int)(x2*fw);
            int x1c = x1a >> 8;
            unsigned int *dsrc2 = &dsrc[y1c * src_width + x1c];
            unsigned int r = 0, g = 0, b = 0, t = 0;
            unsigned int weight_x = 256 - (x1a & 0xFF);
            unsigned int weight_y = 256 - (y1a & 0xFF);
            int y;

            for (y = 0; y < 2; y++) {
               int x;

               for (x = 0; x < 2; x++) {
                  unsigned int c = dsrc2[x + y * src_width];
                  unsigned int b_src = (c    ) & 0xFF;
                  unsigned int g_src = (c >> 8) & 0xFF;
                  unsigned int r_src = (c >> 16) & 0xFF;
                  unsigned int t_src = ignore_alpha ? 255 : (c >> 24) & 0xFF;
                  unsigned int w = (weight_x * weight_y) >> weight_shift;
                  r += r_src * w;
                  g += g_src * w;
                  b += b_src * w;
                  t += t_src * w;
                  weight_x = 256 - weight_x;
               }

               weight_y = 256 - weight_y;
            }

            unsigned int c = ((r >> 16)) | ((g >> 8) & 0xFF00) | (b & 0xFF0000) ;
            *ddest++ = c;
         }
      }
   } else {
      if (g_px1ab_w < dst_width) {
         if (g_px1ab) free(g_px1ab);

         g_px1ab = (int*)malloc(dst_width * 2 * 2 * 4);
         g_px1ab_w = dst_width * 2;
      }

      int x2;

      for (x2 = 0; x2 < dst_width; x2++) {
         int x1a = (int)((x2  ) * fw);
         int x1b = (int)((x2 + 1) * fw);

         if (bUpsampleX)
            x1b = x1a + 256;

         x1b = MIN(x1b, 256 * src_width - 1);
         g_px1ab[x2 * 2 + 0] = x1a;
         g_px1ab[x2 * 2 + 1] = x1b;
      }

      int y2;

      for (y2 = 0; y2 < dst_height; y2++) {
         int y1a = (int)((y2  ) * fh);
         int y1b = (int)((y2 + 1) * fh);

         if (bUpsampleY)
            y1b = y1a + 256;

         y1b = MIN(y1b, 256 * src_height - 1);
         int y1c = y1a >> 8;
         int y1d = y1b >> 8;
         int x2;

         for (x2 = 0; x2 < dst_width; x2++) {
            int x1a = g_px1ab[x2 * 2 + 0];
            int x1b = g_px1ab[x2 * 2 + 1];
            int x1c = x1a >> 8;
            int x1d = x1b >> 8;
            unsigned int r = 0, g = 0, b = 0, a = 0, t = 0;
            int y;

            for (y = y1c; y <= y1d; y++) {
               unsigned int weight_y = 256;

               if (y1c != y1d) {
                  if (y == y1c)
                     weight_y = 256 - (y1a & 0xFF);
                  else if (y == y1d)
                     weight_y = (y1b & 0xFF);
               }

               unsigned int *dsrc2 = &dsrc[y * src_width + x1c];
               int x;

               for (x = x1c; x <= x1d; x++) {
                  unsigned int weight_x = 256;

                  if (x1c != x1d) {
                     if (x == x1c)
                        weight_x = 256 - (x1a & 0xFF);
                     else if (x == x1d)
                        weight_x = (x1b & 0xFF);
                  }

                  unsigned int c = *dsrc2++;
                  unsigned int b_src = (c      ) & 0xFF;
                  unsigned int g_src = (c >>  8) & 0xFF;
                  unsigned int r_src = (c >> 16) & 0xFF;
                  unsigned int t_src = ignore_alpha ? 255 : ((c >> 24) & 0xFF);
                  unsigned int w = (weight_x * weight_y) >> weight_shift;
                  g += g_src * w * t_src / 255;
                  r += r_src * w * t_src / 255;
                  b += b_src * w * t_src / 255 ;
                  t += t_src * w;
                  a += w;
               }
            }

            unsigned int c = 0;

            if (t) {
               c = ((b * 255LL / t)) | ((g * 255LL / t) << 8) | ((r * 255LL / t) << 16) | ((
                        t / a) << 24);
            }

            *ddest++ = c;
         }
      }
   }
}

static void write_jpeg(FILE *out,
                       int quality,
                       int image_width,
                       int image_height,
                       JSAMPLE *image_buffer) {
   unsigned int i, j;
   struct jpeg_compress_struct cinfo;
   struct jpeg_error_mgr jerr;
   JSAMPLE *row_pointer = (JSAMPLE*)malloc(3 * image_width);
   cinfo.err = jpeg_std_error(&jerr);
   jpeg_create_compress(&cinfo);
   jpeg_stdio_dest(&cinfo, out);
   cinfo.image_width = image_width;
   cinfo.image_height = image_height;
   cinfo.input_components = 3;
   cinfo.in_color_space = JCS_RGB;
   jpeg_set_defaults(&cinfo);
   jpeg_set_quality(&cinfo, quality, TRUE);
   jpeg_start_compress(&cinfo, TRUE);

   while (cinfo.next_scanline < cinfo.image_height) {
      unsigned long offset = (image_height - 1 - cinfo.next_scanline) *
                             (image_width << 2);

      for (i = 0; i < image_width; i++)
         for (j = 0; j < 3; j++)
            row_pointer[3 * i + 2 - j] = image_buffer[offset + (i << 2) + j];

      jpeg_write_scanlines(&cinfo, (JSAMPROW*)&row_pointer, 1);
   }

   jpeg_finish_compress(&cinfo);
   jpeg_destroy_compress(&cinfo);
}

static struct read_jpeg_error_struct {
   struct jpeg_error_mgr err;
   jmp_buf jmp_buffer;
} jerr;

static void read_jpeg_error_exit(j_common_ptr cinfo) {
   struct read_jpeg_error_struct* err = (struct read_jpeg_error_struct*)(
                                           cinfo->err);
   longjmp(err->jmp_buffer, 1);
}

struct picture_data* read_and_resize_jpeg(const char* filename, int width,
      int height) {
   FILE* in;
   unsigned char* buffer;
   int i;
   buffer = NULL;
   in = fopen(filename, "rb");

   if (in) {
      struct jpeg_decompress_struct cinfo;
      cinfo.err = jpeg_std_error(&jerr.err);
      jerr.err.error_exit = read_jpeg_error_exit;

      if (setjmp(jerr.jmp_buffer))
         goto end;

      jpeg_create_decompress(&cinfo);
      jpeg_stdio_src(&cinfo, in);
      (void)jpeg_read_header(&cinfo, TRUE);
      (void)jpeg_start_decompress(&cinfo);

      if (cinfo.output_components != 3)
         goto end;

      {
         buffer = (unsigned char*)malloc(3 * cinfo.output_width
                                         * cinfo.output_height +
                                         offsetof(struct picture_data, data));
         unsigned char* ptr = buffer + offsetof(struct picture_data, data);

         while (cinfo.output_scanline < cinfo.output_height) {
            (void)jpeg_read_scanlines(&cinfo, (JSAMPARRAY)&ptr, 1);
            ptr += 3 * cinfo.output_width;
         }

         (void)jpeg_finish_decompress(&cinfo);

         if ((width == 0) && (height == 0)) {
            width = cinfo.output_width;
            height = cinfo.output_height;
         } else {
            if (width == 0) width = cinfo.output_width * height / cinfo.output_height;
            else if (height == 0) height = cinfo.output_height * width  /
                                              cinfo.output_width;
         }

         if ((width == cinfo.output_width) && (height == cinfo.output_height)) {
            ((struct picture_data*)buffer)->bytes_per_pixel = 3;
            ((struct picture_data*)buffer)->width = width;
            ((struct picture_data*)buffer)->height = height;
            goto end;
         }

         unsigned char* src = (unsigned char*)malloc(cinfo.output_width *
                              cinfo.output_height * 4);

         for (i = 0; i < cinfo.output_width * cinfo.output_height; i++) {
            src[(i << 2) + 0] = buffer[3 * i + 0];
            src[(i << 2) + 1] = buffer[3 * i + 1];
            src[(i << 2) + 2] = buffer[3 * i + 2];
            src[(i << 2) + 3] = 0xFF;
         }

         free(buffer);
         buffer = (unsigned char*)malloc(width * height * 4 + offsetof(
                                            struct picture_data, data));
         resize_image(src, cinfo.output_width, cinfo.output_height,
                      buffer + offsetof(struct picture_data, data), width, height, 1);
         free(src);
         ((struct picture_data*)buffer)->bytes_per_pixel = 4;
         ((struct picture_data*)buffer)->width = width;
         ((struct picture_data*)buffer)->height = height;
      }
end:
      jpeg_destroy_decompress(&cinfo);
      fclose(in);
   }

   return (struct picture_data*)buffer;
}

static int choose_filename(const char *dir, char* filename, int num_ss) {
   struct stat buf;
   unsigned int i, m = 0;
   time_t t = (1ull << ((sizeof(time_t) << 3) - 1)) - 1;

   if (num_ss > 1)  {
      for (i = 0; i < num_ss; i++) {
         sprintf(filename, "./%s_%u.jpg", dir, i);

         if (stat(filename, &buf) != 0)
            return 1;

         if (buf.st_mtime < t) {
            t = buf.st_mtime;
            m = i;
         }
      }
   }

   sprintf(filename, "./%s_%u.jpg", dir, m);
   return 0;
}

static int screenshot(const char* dir, int pic_width, int pic_height,
                      int num_ss) {
   void *data;
   int width, height;
   char filename[128];
   FILE* out;
   int ret = 1;
   int x, y, x1, x2, y1, y2;
   unsigned int *ptr;
#ifdef WIN32
   width = GetSystemMetrics(SM_CXSCREEN);
   height = GetSystemMetrics(SM_CYSCREEN);
   data = malloc(width * height * 4);
   HDC screen_dc = CreateCompatibleDC(0);
   HBITMAP bitmap_h = CreateCompatibleBitmap(GetDC(0), width, height);
   HBITMAP old_bitmap_h = (HBITMAP)SelectObject(screen_dc, bitmap_h);
   BitBlt(screen_dc, 0, 0, width, height, GetDC(0), 0, 0, SRCCOPY);
   BITMAPINFO bb;
   bb.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
   bb.bmiHeader.biWidth = width;
   bb.bmiHeader.biHeight = height;
   bb.bmiHeader.biPlanes = 1;
   bb.bmiHeader.biBitCount = 32;
   bb.bmiHeader.biCompression = BI_RGB;
   bb.bmiHeader.biSizeImage = 0;
   GetDIBits(screen_dc, bitmap_h, 0, height, NULL, &bb, DIB_RGB_COLORS);
   GetDIBits(screen_dc, bitmap_h, 0, height, data, &bb, DIB_RGB_COLORS);
   SelectObject(screen_dc, old_bitmap_h);
   DeleteObject(bitmap_h);
   DeleteDC(screen_dc);
#else
   XWindowAttributes gwa;
   unsigned long red_mask;
   unsigned long green_mask;
   unsigned long blue_mask;
   Display *display = XOpenDisplay(NULL);
   Window root = DefaultRootWindow(display);
   XGetWindowAttributes(display, root, &gwa);
   width = gwa.width;
   height = gwa.height;
   XImage *image = XGetImage(display, root, 0, 0, width, height, AllPlanes,
                             ZPixmap);
   data = malloc(4 * width * height);
   red_mask = image->red_mask;
   green_mask = image->green_mask;
   blue_mask = image->blue_mask;
   ptr = (unsigned int*)data;

   for (y = height - 1; y >= 0; y--)
      for (x = 0; x < width; x++) {
         unsigned long pixel = XGetPixel(image, x, y);
         *(ptr++) = pixel & (blue_mask | green_mask | red_mask);
      }

#endif
   /* Auto crop */
   x1 = width - 1;
   x2 = 0;
   y1 = height - 1;
   y2 = 0;
   ptr = (unsigned int*)data;

   for (y = 0; y < height; y++, ptr += width)
      for (x = 0; x < width; x++)
         if (ptr[x] & 0x00FFFFFF) {
            if (x < x1) x1 = x;

            if (x > x2) x2 = x;

            if (y < y1) y1 = y;

            if (y > y2) y2 = y;
         }

   height = y2 - y1 + 1;

   /* Quit if image is too small. */
   if ((x2 - x1 < 100) || (height < 100))
      goto end;

   ret = choose_filename(dir, filename, num_ss);
   out = fopen(filename, "wb");

   if (out) {
      if ((pic_width == 0) && (pic_height == 0)) {
         pic_width = x2 - x1 + 1;
         pic_height = height;
      } else if (pic_width == 0) {
         pic_height = abs(pic_height);
         pic_width = (x2 - x1 + 1) * pic_height / height;
      } else if (pic_height == 0) {
         pic_width = abs(pic_width);
         pic_height = height * pic_width / (x2 - x1 + 1);
      } else if ((pic_width < 0) && (pic_height < 0)) {
         pic_width = -pic_width;
         pic_height = -pic_height;

         if (pic_width * height > pic_height * (x2 - x1 + 1)) {
            pic_width = pic_height * (x2 - x1 + 1) / height;
         } else if (pic_width * height < pic_height * (x2 - x1 + 1)) {
            pic_height = pic_width * height / (x2 - x1 + 1);
         }
      } else {
         if (pic_width < 0) {
            pic_width = MIN((x2 - x1 + 1) * pic_height / height, -pic_width);
         } else if (pic_height < 0) {
            pic_height = MIN(height * pic_width / (x2 - x1 + 1), -pic_height);
         }

         if (pic_width * height < pic_height * (x2 - x1 + 1)) {
            int new_pic_width = pic_width * height / pic_height - 1;
            x1 += (x2 - x1 + 1 - new_pic_width) / 2;
            x2 = new_pic_width + x1 - 1;
         } else if (pic_width * height > pic_height * (x2 - x1 + 1)) {
            height = pic_height * (x2 - x1 + 1) / pic_width;
            y1 += (y2 - y1 + 1 - height) / 2;
            x2 = height + y1 - 1;
         }
      }

      if ((width != x2 - x1 + 1) || (y1 != 0)) {
         ptr = (unsigned int*)data + y1 * width + x1;

         for (x = 0, y = y1; y <= y2; y++, x += x2 - x1 + 1, ptr += width)
            memmove((unsigned int*)data + x, ptr, (x2 - x1 + 1) << 2);

         width = x2 - x1 + 1;
      }

      if  ((pic_width == width) && (pic_height == height)) {
         write_jpeg(out, 100, pic_width, pic_height, (JSAMPLE*)data);
      } else {
         void* dest = malloc(4 * pic_width * pic_height);
         resize_image(data, width, height, dest, pic_width, pic_height, 1);
         write_jpeg(out, 100, pic_width, pic_height, (JSAMPLE*)dest);
         free(dest);
      }

      fclose(out);
   }

end:
   free(data);
   return ret;
}

void parse_conf(const char* conf_filename) {
   FILE *f = NULL;
   f = fopen(conf_filename, "rt");

   if (f) {
      yyin = f;
      n = 0;
      l = 1;
      err = 0;
      war = 0;
      yyparse();
      fclose(f);
   }
}

time_t run_and_wait(const struct entry *ig, int pic_width, int pic_height,
                    int num_ss) {
   time_t start;
   send_map(&ig->map);
   char* t = str_replace(ig->lunc->dir, "$GAME", ig->dir);
   char* dir = str_replace(t, "$EXE", ig->exe);
   free(t);
   t = str_replace(ig->lunc->exe, "$GAME", ig->dir);
   char* cmd = str_replace(t, "$EXE", ig->exe);
   free(t);
   start = time(NULL);
#ifdef WIN32

   do {
      STARTUPINFO si;
      PROCESS_INFORMATION pi;
      ZeroMemory(&si, sizeof(si));
      si.cb = sizeof(si);
      ZeroMemory(&pi, sizeof(pi));

      if (!CreateProcess(NULL, cmd, NULL, NULL, FALSE, 0, NULL, dir, &si, &pi)) {
         printf("CreateProcess failed (%d)\n", (int)GetLastError());
         break;
      }

      if (ig->screen) {
         while (WaitForSingleObject(pi.hProcess,
                                    ((rand() % 3) + 1) * 60000) == WAIT_TIMEOUT)
            screenshot(ig->dir, pic_width, pic_height, num_ss);
      } else WaitForSingleObject(pi.hProcess, INFINITE);

      CloseHandle(pi.hProcess);
      CloseHandle(pi.hThread);
   } while (0);

#else

   do {
      char* par[32];
      char* pch;
      int i;
      sigset_t mask;
      sigset_t orig_mask;
      i = 0;
      pid_t pid;
      pch = strtok(cmd, " ");
      memset(par, 0, 32 * sizeof(char*));

      while (pch != NULL) {
         par[i] = (char*)malloc(strlen(pch) + 1);
         strcpy(par[i++], pch);
         pch = strtok (NULL, " ");
      }

      sigemptyset(&mask);
      sigaddset(&mask, SIGCHLD);
      sigprocmask(SIG_BLOCK, &mask, &orig_mask);
      pid = fork();

      if (pid == -1) break;

      if (pid) {
         int status;
         struct timespec timeout;

         for (i = 0; i < 32; i++)
            if (par[i]) free(par[i]);

         timeout.tv_nsec = 0;

         if (ig->screen) do {
               timeout.tv_sec = ((rand() % 3) + 1) * 60;

               if (sigtimedwait(&mask, NULL, &timeout) < 0)
                  if (errno == EAGAIN) {
                     if (screenshot(ig->dir, pic_width, pic_height, num_ss))
                        continue;
                  }

               break;
            } while (1);

         sigprocmask(SIG_SETMASK, &orig_mask, NULL);
         waitpid(pid, &status, 0);
      } else {
         sigprocmask(SIG_SETMASK, &orig_mask, NULL);

         if (chdir(dir) == 0) {
            execv(par[0], par);
         }

         exit(1);
      }
   } while (0);

#endif
   start = time(NULL) - start;
   free(dir);
   free(cmd);
   send_reset();
   return start;
}

void send_map(const struct full_map *map) {
   unsigned char* buf = (unsigned char*)alloca(sizeof(struct full_map) + 1);
   buf[0] = 0x42;
   memcpy(buf + 1, map, sizeof(struct full_map));
#ifdef WIN32
   DWORD n;
   ULONG length, required;
   PSP_DEVICE_INTERFACE_DETAIL_DATA detailData;
   SP_DEVICE_INTERFACE_DATA devInfoData;
   HANDLE hDevInfo;
   HANDLE handle;
   GUID hidGuid;
   DWORD i;
   HIDD_ATTRIBUTES attr;
   HidD_GetHidGuid(&hidGuid);
   hDevInfo = SetupDiGetClassDevs(&hidGuid, NULL, NULL,
                                  DIGCF_PRESENT | DIGCF_INTERFACEDEVICE);
   devInfoData.cbSize = sizeof(devInfoData);

   for (i = 0; SetupDiEnumDeviceInterfaces(hDevInfo, 0, &hidGuid, i, &devInfoData);
         i++) {
      SetupDiGetDeviceInterfaceDetail(hDevInfo, &devInfoData, NULL, 0, &length, NULL);
      detailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA)malloc(length);
      detailData -> cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
      SetupDiGetDeviceInterfaceDetail(hDevInfo, &devInfoData, detailData, length,
                                      &required, NULL);
      handle = CreateFile(detailData->DevicePath,
                          GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
                          (LPSECURITY_ATTRIBUTES)NULL, OPEN_EXISTING, 0, NULL);

      if (handle) {
         memset(&attr, 0, sizeof(attr));
         attr.Size = sizeof(attr);
         HidD_GetAttributes(handle, &attr);

         if ((attr.VendorID == 0x0000) && (attr.ProductID == 0xAA01)) {
            if (WriteFile(handle, buf, sizeof(struct full_map) + 1, &n, NULL) == FALSE)
               printf("Error sending output report error 0x%lX\n", GetLastError());

            break;
         }

         CloseHandle(handle);
      }

      free(detailData);
   }

#endif
}
