#include <gui/splash_screen/SplashView.hpp>
#include <touchgfx/Color.hpp>
#include <BitmapDatabase.hpp>
#include <texts/TextKeysAndLanguages.hpp>

SplashView::SplashView() :
     moveAnimationEndedCallback(this, &SplashView::moveAnimationEndedHandler)
{

}

void SplashView::setupScreen()
{
    backgroundBox.setPosition(0, 0, touchgfx::HAL::DISPLAY_WIDTH, touchgfx::HAL::DISPLAY_HEIGHT);
    backgroundBox.setColor(Color::getColorFrom24BitRGB(0xff, 0xff, 0xff));

    backgroundImage.setPosition(0, 0, touchgfx::HAL::DISPLAY_WIDTH, touchgfx::HAL::DISPLAY_HEIGHT);
    backgroundImage.setBitmap(Bitmap(BITMAP_FRONT_PAGE_BACKGROUND_ALT_ID));

    logo.setBitmap(Bitmap(BITMAP_LOGO2_ID));
    logo.setXY(touchgfx::HAL::DISPLAY_WIDTH / 2 - logo.getWidth() / 2, 70); //todo: get the util where we can get "half" - BASIC? that all mixins inherit from
    logo.setMoveAnimationEndedAction(moveAnimationEndedCallback);

    start.setTypedText(TypedText(T_MAIN_LOGO));
    //start.setColor(Color::getColorFrom24BitRGB(0x66, 0xCC, 0x00));
    start.setColor(Color::getColorFrom24BitRGB(0x00, 0x80, 0xFF));
    start.setup(250, 10, 70, &EasingEquations::linearEaseInOut);
    start.setXY(touchgfx::HAL::DISPLAY_WIDTH / 2 - logo.getWidth() / 2, 270);
    start.startAnimation(); //start the animation

    marker.setColor(Color::getColorFrom24BitRGB(0xFF, 0x00, 0x00));
    marker.setPosition(100, 100, 20, 20);

    //add components

    add(backgroundBox);
    add(backgroundImage);
    add(logo);
    add(start);

}

void SplashView::moveCursor()
{
    marker.setPosition(50, 50, 100, 100);
    marker.invalidate();
}

void SplashView::moveAnimationEndedHandler(const MoveAnimator<Image>& element)
{
    //todo: Just send for now.
    presenter->send();
}

void SplashView::tearDownScreen()
{

}
