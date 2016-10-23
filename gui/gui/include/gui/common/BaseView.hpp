#ifndef BASE_VIEW_HPP
#define BASE_VIEW_HPP

#include <mvp/View.hpp>
#include <gui/common/BaseViewInterface.hpp>
#include <touchgfx/Color.hpp>
#include <gui/common/defines.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include <gui/common/Time.hpp>
#include <BitmapDatabase.hpp>
#include <gui/common/Util.hpp>
#include <touchgfx/widgets/ButtonWithLabel.hpp>
#include <gui/common/Styles.hpp>
#include <touchgfx/widgets/TextArea.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/Unicode.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>


#ifdef SIMULATOR
#include <stdio.h>
#endif



using namespace touchgfx;


template <class T>
class BaseView : public View<T>, public BaseViewInterface
{
public:
    BaseView()
    {

    }

	virtual void finalizeBaseViewSetup()
	{
      // ?
	}
    
    //util
    struct Point
    {
        uint16_t x;
        uint16_t y;
    };

    //util
    Point getAbsCenter(TextArea text, uint16_t width, uint16_t height)
    {
        Point p;
        uint16_t textHeight = text.getTextHeight();
        uint16_t textWidth = text.getTextWidth();

        p.x = width / 2 - textWidth / 2;
        p.y = height / 2 - textHeight / 2;

        return p;
    }

    void initTextArea(TextArea& textArea, uint16_t x, uint16_t y, TypedTextId textId, colortype textColor)
    {
        textArea.setXY(x, y);
        textArea.setWidth(touchgfx::HAL::DISPLAY_WIDTH);
        textArea.setColor(textColor);
        textArea.setTypedText(TypedText(textId));
    }

    void initButtonWithLabel(ButtonWithLabel& button, uint16_t x, uint16_t y, BitmapId unpressed, BitmapId pressed, TypedTextId labelTextId, colortype unpressedColor, colortype pressedColor, GenericCallback< const AbstractButton& >& action)
    {
        button.setBitmaps(Bitmap(unpressed), Bitmap(pressed));
        button.setXY(x, y);
        button.setLabelText(TypedText(labelTextId));
        button.setLabelColor(unpressedColor);
        button.setLabelColorPressed(pressedColor);
        button.setAction(action);
    }

    TextArea& defineLabel(uint16_t x, uint16_t y, TypedTextId textId, colortype textColor)
    {
        int index = 0;
        bool available = false;
        for (int i = 0; i < 3 /* todo: define max*/; i++)
        {
            TypedText text = texts[i].getTypedText();
            if (text.getId() == TYPED_TEXT_INVALID)
            {
                texts[i].setXY(x, y);
                texts[i].setColor(textColor);
                texts[i].setTypedText(TypedText(textId));

                //Found one: Break out and remember index
                index = i;
                available = true;
                break;
            }
        }

        //todo: if no texts available - assert no available text areas.
        assert(available && "No available textareas found");
        return texts[index];
    }

    int16_t getApplicationWidth() { return 800; }
    int16_t getApplicationHeight() { return 480; }

//	virtual void updateStatusBar(Time time, TimeFormat timeFormat, BatteryIndicatorLevel batteryIndicatorLevel, SoundLevel soundLevel, bool wifi, bool record)
 
    virtual void updateProcessorLoad(uint8_t mcuLoadInProcent)
    {
        Unicode::snprintf(mcuLoadTxtValueBuffer, 5, "%d", mcuLoadInProcent);
        mcuLoadValueTxt.invalidate();
    }

protected:

    Box background;
    
		static const int APPLICATION_MAX_NUMBER_OF_LABELS = 3;
		TextArea texts[APPLICATION_MAX_NUMBER_OF_LABELS];

    // MCU-LOAD
    TextArea mcuLoadTxt;
    TextAreaWithOneWildcard mcuLoadValueTxt;
    Unicode::UnicodeChar mcuLoadTxtValueBuffer[5];
};

#endif
