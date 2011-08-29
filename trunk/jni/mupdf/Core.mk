LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_CFLAGS := -DNOCJK -DNODROIDFONT

LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/../jbig2dec \
	$(LOCAL_PATH)/../openjpeg/libopenjpeg \
	$(LOCAL_PATH)/../jpeg \
	$(LOCAL_PATH)/../freetype/include \
	$(LOCAL_PATH)/draw \
	$(LOCAL_PATH)/fitz \
	$(LOCAL_PATH)/pdf \
	$(LOCAL_PATH)/scripts \

LOCAL_MODULE    := mupdfcore
LOCAL_SRC_FILES := \
	fitz/base_error.c \
	fitz/base_geometry.c \
	fitz/base_getopt.c \
	fitz/base_hash.c \
	fitz/base_memory.c \
	fitz/base_object.c \
	fitz/base_string.c \
	fitz/base_time.c \
	fitz/crypt_aes.c \
	fitz/crypt_arc4.c \
	fitz/crypt_md5.c \
	fitz/crypt_sha2.c \
	fitz/dev_bbox.c \
	fitz/dev_list.c \
	fitz/dev_null.c \
	fitz/dev_text.c \
	fitz/dev_trace.c \
	fitz/filt_basic.c \
	fitz/filt_dctd.c \
	fitz/filt_faxd.c \
	fitz/filt_flate.c \
	fitz/filt_jbig2d.c \
	fitz/filt_jpxd.c \
	fitz/filt_lzwd.c \
	fitz/filt_predict.c \
	fitz/obj_print.c \
	fitz/res_colorspace.c \
	fitz/res_font.c \
	fitz/res_path.c \
	fitz/res_pixmap.c \
	fitz/res_shade.c \
	fitz/res_text.c \
	fitz/stm_buffer.c \
	fitz/stm_open.c \
	fitz/stm_read.c \
	draw/arch_arm.c \
	draw/arch_port.c \
	draw/draw_affine.c \
	draw/draw_blend.c \
	draw/draw_device.c \
	draw/draw_edge.c \
	draw/draw_glyph.c \
	draw/draw_mesh.c \
	draw/draw_paint.c \
	draw/draw_path.c \
	draw/draw_scale.c \
	draw/draw_unpack.c \
	pdf/pdf_annot.c \
	pdf/pdf_cmap.c \
	pdf/pdf_cmap_load.c \
	pdf/pdf_cmap_parse.c \
	pdf/pdf_cmap_table.c \
	pdf/pdf_colorspace.c \
	pdf/pdf_crypt.c \
	pdf/pdf_encoding.c \
	pdf/pdf_font.c \
	pdf/pdf_fontfile.c \
	pdf/pdf_function.c \
	pdf/pdf_image.c \
	pdf/pdf_interpret.c \
	pdf/pdf_lex.c \
	pdf/pdf_metrics.c \
	pdf/pdf_nametree.c \
	pdf/pdf_outline.c \
	pdf/pdf_page.c \
	pdf/pdf_parse.c \
	pdf/pdf_pattern.c \
	pdf/pdf_repair.c \
	pdf/pdf_shade.c \
	pdf/pdf_store.c \
	pdf/pdf_stream.c \
	pdf/pdf_type3.c \
	pdf/pdf_unicode.c \
	pdf/pdf_xobject.c \
	pdf/pdf_xref.c

LOCAL_LDLIBS    := -lz -lm -llog

include $(BUILD_STATIC_LIBRARY)
