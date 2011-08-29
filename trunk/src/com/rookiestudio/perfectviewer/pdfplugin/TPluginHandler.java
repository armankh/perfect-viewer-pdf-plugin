package com.rookiestudio.perfectviewer.pdfplugin; 

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.zip.ZipEntry;
import java.util.zip.ZipFile;

import android.util.Log;

public class TPluginHandler
{
  public static native int PluginOpenFile(String FileName,String Password);
  public static native void PluginGotoPage(int PageNum);
  public static native float PluginPageWidth();
  public static native float PluginPageHeight();
  public static native void PluginCloseFile();
  public static native void PluginDrawPage(int bitmap,int pageW, int pageH);
  
  private String InstallFolder="";
  private String AppDataFolder="";
  public TPluginHandler(boolean HaveNeon,String Path1,String Path2)
  {
    InstallFolder = Path1;
    AppDataFolder = Path2;
    if (HaveNeon) {
      ZipExtractFile(InstallFolder,"assets/libpdf_plugin_neon.so",AppDataFolder+"/libpdf_plugin_neon.so");
      System.load(AppDataFolder+"/libpdf_plugin_neon.so");
    } else {
      ZipExtractFile(InstallFolder,"assets/libpdf_plugin.so",AppDataFolder+"/libpdf_plugin.so");
      System.load(AppDataFolder+"/libpdf_plugin.so");
    }
  }
  
  //extract file from apk file
  public static boolean ZipExtractFile(String ZipFile,String SrcFile,String DstFile) 
  {
    ZipFile zip;
    try {
      File DstFileObj=new File(DstFile);
      zip = new ZipFile(ZipFile);
      ZipEntry zipen = zip.getEntry(SrcFile);      
      if ((!DstFileObj.exists()) || (zipen.getSize()!=DstFileObj.length())) {
        Log.e("pdfplugin","update native library : "+DstFile);
        InputStream is = zip.getInputStream(zipen);
        OutputStream os = new FileOutputStream(DstFile);
        byte[] buf = new byte[8092];
        int n;
        while ((n = is.read(buf)) > 0) {
          os.write(buf, 0, n);
        }
        os.close();
        is.close();
      }
    } catch (IOException e) {
      return false;
    }
    return true;
  }
  
  public static boolean IsFileSupport(String FileName)
  {
    return FileName.endsWith(".pdf");
  }
}
