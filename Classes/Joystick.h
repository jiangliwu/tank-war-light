#include "cocos2d.h" 
#include "type.h"
using namespace cocos2d; 
  
class Joystick : public CCLayer 
{ 
public: 
    Joystick(void); 
    ~Joystick(void); 
public: 
    CCPoint centerPoint;    // ҡ������ 
    CCPoint currentPoint;   // ҡ�˵�ǰλ�� 
    bool active;            // �Ƿ񼤻�ҡ�� 
    float radius;           // ҡ�˰뾶 
    CCSprite *jsSprite;     // ҡ��ʵ�� 
	CCSprite *jsBg;
    //************************************ 
    // Method:    Active 
    // FullName:  Joystick::Active 
    // Access:    public  
    // Returns:   void 
    // Qualifier: ����ҡ�˹��ܿ��� 
    //************************************ 
    void Active(); 
    //************************************ 
    // Method:    Inactive 
    // FullName:  Joystick::Inactive 
    // Access:    public  
    // Returns:   void 
    // Qualifier: ����ҡ�˹��ܲ����� 
    //************************************ 
    void Inactive(); 
    //************************************ 
    // Method:    getDirection 
    // FullName:  Joystick::getDirection 
    // Access:    public  
    // Returns:   cocos2d::CCPoint 
    // Qualifier: ��ȡҡ�˷������ﷵ�ص��ǵ�λ���� 
    //************************************ 
    CCPoint getDirection(); 


	//////////////////////////////////////////////////////////////////////////
	//getDirectionByDir();
	//
	//////////////////////////////////////////////////////////////////////////

	Direction getDirectionByDir();

    //************************************ 
    // Method:    getVelocity 
    // FullName:  Joystick::getVelocity 
    // Access:    public  
    // Returns:   float 
    // Qualifier: ��ȡҡ�˵����� 
    //************************************ 
    float getVelocity(); 
    //************************************ 
    // Method:    updatePos 
    // FullName:  Joystick::updatePos 
    // Access:    public  
    // Returns:   void 
    // Qualifier: ˢ�º����������ճ̹����� 
    // Parameter: ccTime dt 
    //************************************ 
    void updatePos(float dt); 
  
    //************************************ 
    // Method:    JoystickWithCenter 
    // FullName:  Joystick::JoystickWithCenter 
    // Access:    public static  
    // Returns:   Joystick* 
    // Qualifier: ��ʼ��ҡ�� 
    // Parameter: CCPoint aPoint ҡ������ 
    // Parameter: float aRadius ҡ�˰뾶 
    // Parameter: CCSprite * aJsSprite ҡ�˿��Ƶ� 
    // Parameter: CCSprite * aJsBg ҡ�˱��� 
    //************************************ 

	void setOpacity(float op);
    static Joystick* JoystickWithCenter(CCPoint aPoint, float aRadius, CCSprite* aJsSprite, CCSprite* aJsBg); 
    Joystick* initWithCenter(CCPoint aPoint, float aRadius, CCSprite* aJsSprite, CCSprite* aJsBg); 
  
    
	
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent); 
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent); 
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent); 
  
    CREATE_FUNC(Joystick); 
};