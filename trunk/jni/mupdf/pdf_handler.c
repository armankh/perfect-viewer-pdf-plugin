  #include <jni.h>
#include <time.h>
#include <android/log.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "fitz.h"
#include "mupdf.h"

#define LOG_TAG "pdfplugin"
#define LOGD(...)
#define LOGI(...)
#define LOGE(...)
/*#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)*/

/* Set to 1 to enable debug log traces. */
#define DEBUG 0

/* Globals */
fz_colorspace *colorspace;
fz_glyph_cache *glyphcache;
pdf_xref *xref;
int FileOpened=0;
int pagenum = 1;
int resolution = 160;
float pageWidth = 100;
float pageHeight = 100;
fz_display_list *currentPageList;
fz_rect currentMediabox;
int currentRotate;
int CurrentTotalPage=0;
char CurrentFileName[512];

int OpenPDF(const char* filename,const char* password)
{
  fz_error error;
  fz_accelerate();
  glyphcache = fz_new_glyph_cache();
  colorspace = fz_device_rgb;

  LOGD("Opening document...");
  error = pdf_open_xref(&xref, filename, (char*)password);
  if (error) {
    LOGE("Cannot open document: '%s' %d\n", filename,error);
    return 0;
  }

  LOGD("Loading page tree...");
  error = pdf_load_page_tree(xref);
  if (error) {
    LOGE("Cannot load page tree: '%s'\n", filename);
    return 0;
  }
  CurrentTotalPage = pdf_count_pages(xref);
  LOGD("Done! %d pages", CurrentTotalPage);
  FileOpened=1;

  return CurrentTotalPage;
}

void ClosePDF()
{
  fz_free_display_list(currentPageList);
  currentPageList = NULL;
  pdf_free_xref(xref);
  xref = NULL;
  fz_free_glyph_cache(glyphcache);
  glyphcache = NULL;
  FileOpened=0;
}

JNIEXPORT int JNICALL
Java_com_rookiestudio_perfectviewer_pdfplugin_TPluginHandler_PluginOpenFile(JNIEnv * env, jobject thiz, jstring jfilename, jstring jpassword)
{
  const char *filename;
  filename = (*env)->GetStringUTFChars(env, jfilename, NULL);
  const char *password;
  password = (*env)->GetStringUTFChars(env, jpassword, NULL);
  if (FileOpened==1) {
    if (strcmp(filename,CurrentFileName)==0) {
      LOGI("Use current PDF");
      return CurrentTotalPage;
    } else {
      LOGI("Close current PDF");
      ClosePDF();
    }
  }
  strcpy((char*)&CurrentFileName,filename);
	return OpenPDF(filename,password);
}

JNIEXPORT void JNICALL
Java_com_rookiestudio_perfectviewer_pdfplugin_TPluginHandler_PluginGotoPage(JNIEnv *env, jobject thiz, int page)
{
	float zoom;
	fz_matrix ctm;
	fz_bbox bbox;
	fz_error error;
	fz_device *dev;
	pdf_page *currentPage;

	/* In the event of an error, ensure we give a non-empty page */
	pageWidth = 100;
	pageHeight = 100;

	LOGD("Goto page %d...", page);
	if (currentPageList != NULL) {
		fz_free_display_list(currentPageList);
		currentPageList = NULL;
	}
	pagenum = page;
	error = pdf_load_page(&currentPage, xref, pagenum);
	if (error) {
		return;
	}
	zoom = resolution / 72;
	currentMediabox = currentPage->mediabox;
	currentRotate = currentPage->rotate;
	ctm = fz_translate(0, -currentMediabox.y1);
	ctm = fz_concat(ctm, fz_scale(zoom, -zoom));
	ctm = fz_concat(ctm, fz_rotate(currentRotate));
	bbox = fz_round_rect(fz_transform_rect(ctm, currentMediabox));
	pageWidth = bbox.x1-bbox.x0;
	pageHeight = bbox.y1-bbox.y0;
	/* Render to list */
	currentPageList = fz_new_display_list();
	dev = fz_new_list_device(currentPageList);
	error = pdf_run_page(xref, currentPage, dev, fz_identity);
	pdf_free_page(currentPage);
	if (error) {
		LOGE("cannot make displaylist from page %d", pagenum);
	}
	fz_free_device(dev);
}

JNIEXPORT float JNICALL
Java_com_rookiestudio_perfectviewer_pdfplugin_TPluginHandler_PluginPageWidth(JNIEnv *env, jobject thiz)
{
	return pageWidth;
}

JNIEXPORT float JNICALL
Java_com_rookiestudio_perfectviewer_pdfplugin_TPluginHandler_PluginPageHeight(JNIEnv *env, jobject thiz)
{
	return pageHeight;
}

JNIEXPORT jboolean JNICALL
Java_com_rookiestudio_perfectviewer_pdfplugin_TPluginHandler_PluginDrawPage(JNIEnv *env, jobject thiz, int bitmap,
		int pageW, int pageH)
{
	void *pixels=(void*)bitmap;
	int ret;
	fz_error error;
	fz_device *dev;
	float zoom;
	fz_matrix ctm;
	fz_bbox bbox;
	fz_pixmap *pix;
	fz_bbox rect;

	LOGE("Rendering page=%dx%d",pageW, pageH);

	rect.x0 = 0;
	rect.y0 = 0;
	rect.x1 = pageW;
	rect.y1 = pageH;
	pix = fz_new_pixmap_with_rect_and_data(colorspace, rect, pixels);
	if (currentPageList == NULL)
	{
		fz_clear_pixmap_with_color(pix, 0xd0);
		return 0;
	}
	fz_clear_pixmap_with_color(pix, 0xff);

	zoom = resolution / 72;
	ctm = fz_translate(-currentMediabox.x0, -currentMediabox.y1);
	ctm = fz_concat(ctm, fz_scale(zoom, -zoom));
	ctm = fz_concat(ctm, fz_rotate(currentRotate));
	bbox = fz_round_rect(fz_transform_rect(ctm,currentMediabox));

	ctm = fz_concat(ctm, fz_scale(1, 1));
	bbox = fz_round_rect(fz_transform_rect(ctm,currentMediabox));
	dev = fz_new_draw_device(glyphcache, pix);
	fz_execute_display_list(currentPageList, dev, ctm, bbox);
	fz_free_device(dev);
	fz_drop_pixmap(pix);
	LOGD("Rendered");

	return 1;
}

JNIEXPORT void JNICALL
Java_com_rookiestudio_perfectviewer_pdfplugin_TPluginHandler_PluginCloseFile(JNIEnv * env, jobject thiz)
{
  ClosePDF();
}
