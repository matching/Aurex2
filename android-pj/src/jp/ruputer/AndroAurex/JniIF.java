package jp.ruputer.AndroAurex;

import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Rect;
import android.util.Log;
import android.view.SurfaceView;

public class JniIF implements Runnable {

	public static final int BUTTON_ENTER = 0x0008;
	public static final int BUTTON_EL    = 0x0004;
	public static final int BUTTON_FILER = 0x0001;
	public static final int BUTTON_MENU  = 0x0002;

	public static final int BUTTON_DOWN  = 0x0080;
	public static final int BUTTON_UP    = 0x0020;
	public static final int BUTTON_RIGHT = 0x0040;
	public static final int BUTTON_LEFT  = 0x0010;
	
	public Bitmap bitmap;
	SurfaceView sv;
	Rect src, dst;
	int expansion;
	
	int nullcounter;
	
	public JniIF(SurfaceView sv, int expansion) {
		
		this.expansion = expansion;

		this.sv = sv;
		this.bitmap = Bitmap.createBitmap(102 * expansion, 64 * expansion, Bitmap.Config.RGB_565);
//		this.bitmap.setDensity( 120 );

//		Log.v("androaurex", "Jniif constructor ", );

		src = new Rect(0, 0, 102 * expansion, 64 * expansion);
		dst = new Rect(0, 0, 102 * expansion, 64 * expansion);		

		nullcounter++;
	
	}

	public native void pushButtonDown(int key);
	public native void pushButtonUp(int key);
	
	public native void aurexjnimain(Bitmap bitmap, int expansion);
	public native void aurexjniresume();
	public native void aurexjnipause();

	
	@Override
	public void run() {
		aurexjnimain(bitmap, expansion);
	}

	public void drawBitmapToCanvas() {

		Canvas canvas;
				
		canvas = sv.getHolder().lockCanvas();
		if( canvas == null ) {
			Log.v("androaurex", "Java canvas is null!!!!!!! " + ++nullcounter);
		}
		else {

//			canvas.scale(1, 1);
//			canvas.setDensity( 160 );

			//canvas.drawBitmap(bitmap, src, dst, null);
			canvas.drawBitmap(bitmap, 0, 0, null);
			
			sv.getHolder().unlockCanvasAndPost(canvas);
		}
//		Log.v("androaurex", "Java Method drai Bitmap to canvas called!!!!!!!");

		
	}
	
}
