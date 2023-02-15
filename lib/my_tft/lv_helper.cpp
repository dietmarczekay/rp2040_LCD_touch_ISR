#include "lv_helper.h"

TFT_eSPI lh_tft = TFT_eSPI();

static lv_disp_draw_buf_t lh_draw_buf;
static lv_color_t lh_buf[ DISPLAY_WIDTH * 10 ];
static lv_disp_drv_t lh_disp_drv;
static lv_indev_drv_t lh_indev_drv;

uint16_t width, height;

void lh_init(int rotation)
{
  Wire.begin();
  //lh_tp.begin();
  lv_init();
  lh_tft.begin();
  if (rotation == 1 || rotation == 3){
    width = DISPLAY_HEIGHT;
    height = DISPLAY_WIDTH;
  } else {
    width = DISPLAY_WIDTH;
    height = DISPLAY_HEIGHT;
  }
  lh_tft.setRotation(rotation);
  //lh_tp.setRotation(rotation);

  lv_disp_draw_buf_init( &lh_draw_buf, lh_buf, NULL, DISPLAY_WIDTH * 10 );

  /*Initialize the display*/
  lv_disp_drv_init( &lh_disp_drv );
  /*Change the following line to your display resolution*/
  lh_disp_drv.hor_res = width;
  lh_disp_drv.ver_res = height;
  lh_disp_drv.flush_cb = lh_disp_flush;
  lh_disp_drv.draw_buf = &lh_draw_buf;
  lv_disp_drv_register( &lh_disp_drv );

  /*Initialize the (dummy) input device driver*/
  lv_indev_drv_init( &lh_indev_drv );
  lh_indev_drv.type = LV_INDEV_TYPE_POINTER;
  lh_indev_drv.read_cb = lh_touchpad_read;
  lv_indev_drv_register( &lh_indev_drv );
}

/* Display flushing */
void lh_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) 
{
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

  lh_tft.startWrite();
  lh_tft.setAddrWindow( area->x1, area->y1, w, h );
  lh_tft.pushColors( ( uint16_t * )&color_p->full, w * h, true );
  lh_tft.endWrite();

  lv_disp_flush_ready(disp);
}

/*Read the touchpad*/
void lh_touchpad_read(lv_indev_drv_t * indev_driver, lv_indev_data_t * data) 
{
    uint16_t x, y;
    bool touched = lh_tft.getTouch(&x, &y);
    if (!touched)
    {
      data->state = LV_INDEV_STATE_RELEASED;
    }
    else
    {
      data->state = LV_INDEV_STATE_PRESSED;
      data->point.x = x;
      data->point.y = y;
    }
}
