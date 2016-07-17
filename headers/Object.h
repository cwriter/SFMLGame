#pragma once
#include "stdafx.h"
#include "Message.h"
#define _CRTDBG_MAP_ALLOC
#include "XML.h"
#include "GameState.h"
#include "Context.h"

class Loadable;
class Module;
class GObjectBase;



class bytearr {
public:
    bytearr() {
        len = 0;
        data = NULL;
    }
    ~bytearr() {
        if (data != NULL) {
            free(data);
        }
    }
    size_t len;
    char* data;
};
///<summary>
///General loadable that's managable.
///</summary>
class Loadable
{
public:
    enum LoadableMessages
    {
        Loaded = 0,
        Unloaded,
        Preloaded,
        Destroyed,

        LoadableMessagesCount
    };
    Loadable() {
        this->m_governor = NULL;
        loaded = false;
    }

    Loadable(Module* gov);

    virtual ~Loadable() {
        destroy();
    }

    virtual int preload() {
        return 0;
    }

    virtual int load(const char* data) {

        return 0;
    }

    virtual int load(XMLReader& data) {

        return 0;
    }

    virtual void unload() {

    }

    ///<summary>
    ///Creates a new instance of a certain type from an existing instance
    ///</summary>
    ///<returns>
    ///An SFG Pointer holding the freshly generated instance
    ///</returns>
    virtual SFG::Pointer<Loadable> clone()
    {
        //Clients need to reimplement this function
        assert(false);
        return SFG::Pointer<Loadable>(nullptr);
    }

    ///<summary>
    ///Creates a chunk of memory from the object. <br/>
    ///The object shall always be able to be rebuilt using the memory information.
    ///</summary>
    virtual char* createMemoryChunk(size_t* size);

    virtual void destroy() {

    }

//protected:
    bool loaded;
    Module* m_governor;

    XMLReader m_xml_source;
    sf::String m_source;
    sf::String m_name;
};

class Texture : public Loadable
{
public:

    Texture(Module* gov) : Loadable(gov) {
        m_texture.reset(nullptr);
    }

    Texture() {
        this->m_governor = NULL;
        m_texture.reset(nullptr);
    }

    virtual char* createMemoryChunk(size_t* size) override;

    int load(const char* data) override {

        return 0;
    }

    void unload() override {

    }

    void destroy() override {
        /*if (m_texture != NULL) delete m_texture;
        m_texture = NULL;*/
    }

    inline void setGovernor(Module* gov) {
        this->m_governor = gov;
    }

    int parseSourceString(const sf::String& str);

    inline operator sf::Texture&() {
        return *this->m_texture.getElement();
    }

    void setTexture(sf::Texture& t) {
        this->m_texture.reset(&t);
    }
    void setTexture(sf::Texture* t) {
        this->m_texture.reset(t);
    }

    sf::Texture& getTexture() {
        return *m_texture.getElement();
    }

private:
    SFG::Pointer<sf::Texture> m_texture;
};

class Frame : public Loadable
{
public:
    Frame(Module* gov) : Loadable(gov) {
        m_tex.reset(new Texture(gov));
    }
    ~Frame() {
        destroy();
    }

    void unload() override {
        m_tex->unload();
    }

    void destroy() override {
        m_tex->destroy();
    }

    virtual char* createMemoryChunk(size_t* size) override;

    int load(const char* data) override {
        printf("[Error] Undefined function called in %s:%d\n", __FILE__, __LINE__);
        return 0;
    }

    int load(XMLReader& data) override;

    void createFromTexture(Texture& tex, float time) {
        this->m_tex = tex;
        this->m_time = time;
    }

//private:
    SFG::Pointer<Texture> m_tex;
    SFG::IntRect m_tex_rect;
    float m_time;

};

class Effect
{
public:
    Effect()
    {
        m_start = 0.f;
        m_end = 0.f;
    }
    ///<summary>
    ///Plays the effect over an animation
    ///</summary>
    ///<param name="target">The target sprite to apply the effect to</param>
    ///<param name="time">The current time in the animation (not the dt)</param>
    virtual void play(sf::Sprite& target, float time)
    {
        printf("[Error] Effect::play() has to be overridden %s:%d\n", __FILE__, __LINE__);
    }

    virtual void setSpan(float start, float end)
    {
        m_start = start;
        m_end = end;
    }

protected:
    float m_start; //The total time in the
    float m_end;
};

///<summary>
///A simple Fade-out effect with constant alpha scaling
///</summary>
class FadeOutEffect : public Effect
{
public:
    FadeOutEffect()
    {
        m_new_area = nullptr;
    }

    void play(sf::Sprite& target, float time) override;

    void setSpan(float start, float end) override
    {
        m_start = start;
        m_end = end;

        m_stepping = 1.0f/(m_end - m_start);
    }
protected:
    float m_stepping; //Per-dt alpha stepping
    sf::Uint8* m_new_area;
    sf::Image img; //The original image (has to be reapplied once the effect stops)
};


class Animation : public Loadable, public MsgObject
{
public:
    enum AnimationMessages
    {
        AnimationStarted,
        AnimationChanged,
        AnimationFinished,

        AnimationMessagesSize
    };
    Animation(Module* gov) : Loadable(gov) {
        m_anim_state = 0.0f;
        m_totaltime = 0;
        m_current_frame = 0;
    }
    virtual ~Animation() {
        unload();
    }

    void unload() override {
        m_frames.clear();
    }

    int load(XMLReader& data) override;

    virtual char* createMemoryChunk(size_t* size) override;

    void updateAnimation(float dt, bool restart);

    SFG::Pointer<Frame> getCurrentFrame() const
    {
        return this->m_frames[m_current_frame];
    }

    void addFrame(const SFG::Pointer<Frame>& frame) {
        m_totaltime += frame->m_time;
        this->m_frames.push_back(frame);
    }

    void addEffect(const Effect& effect)
    {
        m_effects.push_back(effect);
    }

    size_t m_current_frame;
    float m_totaltime;
    std::vector<SFG::Pointer<Frame>> m_frames;
    std::vector<Effect> m_effects;
    float m_anim_state;
};

class GTextBase : public Loadable
{
public:
    GTextBase() {

    }

    ~GTextBase() {

    }

    int load(XMLReader& xmldata) override;

private:
    sf::String m_string;

    float m_size;
    sf::Font m_font;
    sf::Text m_text;
};


///<summary>
///A general 2d object, to be loaded from xml data
///</summary>
class GObjectBase : public Loadable, public virtual MsgObject
{
public:
    enum GObjectMessages {
        Loaded = LoadableMessages::Loaded,
        Unloaded = LoadableMessages::Unloaded,
        Preloaded = LoadableMessages::Preloaded,
        Destroyed = LoadableMessages::Destroyed,

        PositionChanged = LoadableMessagesCount,
        RotationChanged,
        ScaleChanged,
        SizeChanged,
        TextureChanged,
        AnimationChanged,
        AnimationStarted,
        AnimationFinished,

        PreDraw,
        PostDraw,

        GObjectMessagesCount
    };

    enum ResizePolicy {
        Scale,
        Repeat


    };
    GObjectBase(Module* gov) : Loadable(gov) {
        m_current_animation = 0;
    }
    GObjectBase() {
        this->m_governor = NULL;
        loaded = false;
        m_current_animation = 0;
    }
    virtual ~GObjectBase();
    virtual void unload() override;
    virtual void destroy() override;
    virtual int load(const char* data) override {
        printf("This load operation is not supported\n");
        return 0;
    }

    int notify(int msg) override {

        return 0;
    }

    virtual SFG::Pointer<Loadable> clone() override;

    virtual char* createMemoryChunk(size_t* size) override;

    ///<summary>
    ///Returns all context options for this object that's reachble from point pos
    ///</summary>
    SFG::Pointer<ContextOptionList> getContextOptions(const sf::Vector3f& pos)
    {
        SFG::Pointer<ContextOptionList> ret(new ContextOptionList());
        printf("[Warning] Unimplemented function called in %s:%d\n", __FILE__, __LINE__);
        return ret;
    }

    virtual int load(XMLReader& data) override;

    virtual int preload() override {
        //this->m_xml_source;
        return 0;
    }

    ///<summary>
    ///Updates the current animation
    ///</summary>
    virtual int update(float dt);

    ///<summary>
    ///Sets the size of an object by simply scaling it.
    ///</summary>
    ///<param name="x">The new width</param>
    ///<param name="y">The new height</param>
    ///<param name="policy">The policy to use (scale or repeat). If the object's policy does not agree, this function returns false (true otherwise)</param>
    virtual bool setSize(float x, float y, ResizePolicy policy = ResizePolicy::Scale);

    inline const sf::Sprite& getSprite() const {
        return this->m_sprite;
    }

    virtual void draw(sf::RenderTarget& target);

    /*virtual sf::FloatRect getBoundingRect() const {
    	return this->m_sprite.getGlobalBounds();
    }*/
    SFG::FloatRect getBoundingRect() const {
        return this->m_sprite.getGlobalBounds();
    }

    virtual int playAnimation(const sf::String& name);

    virtual void setPosition(float x, float y) {
        this->m_sprite.setPosition(x, y);
        notifyAll(GObjectMessages::PositionChanged);
    }

    virtual void setPosition(const sf::Vector2f& position) {
        this->m_sprite.setPosition(position);
        notifyAll(GObjectMessages::PositionChanged);
    }

    virtual void setScale(float factorX, float factorY) {
        this->m_sprite.setScale(factorX, factorY);
        notifyAll(GObjectMessages::ScaleChanged);
    }

    virtual void setScale(const sf::Vector2f& factors) {
        this->m_sprite.setScale(factors);
        notifyAll(GObjectMessages::ScaleChanged);
    }

    virtual void setRotation(float angle) {
        m_sprite.setRotation(angle);
        notifyAll(GObjectMessages::RotationChanged);
    }
    
    virtual float getRotation() const {
		return m_sprite.getRotation();
	}

    void setGraphicsName(const sf::String& str) {
        this->m_graphics_name = str; //#TODO: Evalute if necessary
        this->m_name = str;
    }

    const sf::String& getGraphicsName() {
        return this->m_graphics_name;
    }

    virtual int addAnimation(const SFG::Pointer<Animation>& ptr);

    inline float getDepth() {
        return m_depth;
    }

    size_t getCurrentAnimationIndex() const;

    std::vector<SFG::Pointer<Animation>>* getAnimationVector();
private:
    //===== SIGNALS TO LISTEN TO =========
    //Handle signals coming from animations
    int signals_animation(int msg, const SFG::Pointer<MsgPackage>& info);



    sf::String m_graphics_name; //This a the general class name, not a specific, unique name
    std::vector<SFG::Pointer<Animation>> m_animations;
    size_t m_current_animation;
    sf::Sprite m_sprite;
    float m_depth;	//Represents half a dimension, i.e. 2.5d

    //std::vector<MsgObject*> m_listeners;
};
