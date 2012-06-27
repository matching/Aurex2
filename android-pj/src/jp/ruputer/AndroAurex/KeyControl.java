package jp.ruputer.AndroAurex;

import android.util.Log;
import android.view.MotionEvent;
import android.view.View;

public class KeyControl implements View.OnTouchListener {

	private View v;
	private int width, height;
	private int pushKey, prevPushKey;
	private static final int threadshold_sq  = 100;
	JniIF jniif;
	
	public KeyControl(JniIF jniif, View v){
		this.v = v;
		pushKey = 0;		
		prevPushKey = 0;
		this.jniif = jniif;
	}
	
	@Override
	public boolean onTouch(View v, MotionEvent event) {
    	int pushed = 0x0;
    	String str_axis;
   		float x;
   		float y;
    	double distance_sq;
    	
   		x = event.getX();
   		y = event.getY();

   		width = v.getWidth();
   		height = v.getHeight();
   		
   		double t;
 		/*
   		distance_sq = ( height / 2 - y ) * ( height / 2 - y ) +
   		              ( x - width / 2  ) * ( x - width / 2 );
   		if( distance_sq < threadshold_sq ) {

   			if( prevPushKey != 0 ) {
  		   		jniif.pushButtonUp( prevPushKey );
	   		}
   		   	prevPushKey = 0;
   			return true;
   		}
   		*/
   		
   		t = Math.toDegrees( Math.atan2( height / 2 - y, x - width / 2 ) );
   		if( t < 0 )
   			t += 360;
   		
   		if( t > 45 + 45 + 45 + 45 + 45 + 45 + 45 + 22.5 || t <= 22.5 ) { // right
   			pushKey = JniIF.BUTTON_RIGHT;
   		}
   		else if( t > 22.5 && t <= 45 + 22.5 ) { // right up
   			pushKey = JniIF.BUTTON_RIGHT | JniIF.BUTTON_UP;		
   		}
   		else if( t <= 45 + 45 + 22.5 ) { // up 
   			pushKey = JniIF.BUTTON_UP;		   			
   		}
   		else if( t <= 45 + 45 + 45 + 22.5 ) { // left up
   			pushKey = JniIF.BUTTON_LEFT | JniIF.BUTTON_UP;		   	   			
   		}
   		else if( t <= 45 + 45 + 45 + 45 + 22.5 ) { // left
   			pushKey = JniIF.BUTTON_LEFT;	   	   			
   		}
   		else if( t <= 45 + 45 + 45 + 45 + 45 + 22.5 ) { // left down
   			pushKey = JniIF.BUTTON_LEFT | JniIF.BUTTON_DOWN;   	   			
   		}
   		else if( t <= 45 + 45 + 45 + 45 + 45 + 45 + 22.5 ) { // down
   			pushKey = JniIF.BUTTON_DOWN;   	   			
   		}
   		else if( t <= 45 + 45 + 45 + 45 + 45 + 45 + 45 + 22.5 ) { // right down
   			pushKey = JniIF.BUTTON_RIGHT | JniIF.BUTTON_DOWN;   	   			
   		}

   		str_axis = "(" + x + "," + y + "," +  (x - width / 2) + ","  + (height / 2 - y) + "," +  t + ")" ;
   		Log.v("androaurex", "Activity.onTouchEvent " + str_axis);
   		
   		
		if (event.getAction() == MotionEvent.ACTION_DOWN) {
	   		Log.v("androaurex", "Activity.onTouchEvent / control Down " + str_axis);

	   		jniif.pushButtonDown( pushKey );
//	   		jniif.pushButtonUp( pushKey );
	   		prevPushKey = pushKey;
	    }
		else if (event.getAction() == MotionEvent.ACTION_UP) {
	    	Log.v("androaurex", "Activity.onTouchEvent/ control up "  + str_axis );
//	    	jniif.pushButtonUp( JniIF.BUTTON_FILER );
	   		jniif.pushButtonUp( prevPushKey );
	   		prevPushKey = 0;
	   	}
		else if (event.getAction() == MotionEvent.ACTION_MOVE) {
	    	Log.v("androaurex", "Activity.onTouchEvent/ control move "  + str_axis );
//	    	jniif.pushButtonUp( JniIF.BUTTON_FILER );

	   		if( pushKey != prevPushKey ) {
	   			jniif.pushButtonUp( prevPushKey & ~pushKey );
		    	jniif.pushButtonDown( pushKey & ~prevPushKey );
	   		}
/*
	   		jniif.pushButtonDown( pushKey );
	   		jniif.pushButtonUp( pushKey );
	   		*/
	   		prevPushKey = pushKey;
		}
		return true;
	}
}
