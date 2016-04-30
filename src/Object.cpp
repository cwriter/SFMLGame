#include "stdafx.h"
#include "Object.h"

#ifndef _WIN32
#define __FUNCSIG__ __func__
#endif // _WIN32

Loadable::Loadable(Module* gov)
{
	this->m_governor = gov;
	//gov->addObject(this);
	loaded = false;
}

char* Loadable::createMemoryChunk(size_t* size)
{
#ifdef _DEBUG
	printf("[Critical] Override missing for %s in %s:%d", __FUNCSIG__, __FILE__, __LINE__);
#ifdef _WIN32
	DebugBreak();
#endif
#endif
	*size = 0;
	return NULL;
}

char* Texture::createMemoryChunk(size_t* size)
{
	*size = strlen("TEXTURE:") + this->m_source.getSize() + 1;
	char* data = (char*)malloc(*size);
	if (data == NULL)
		return NULL;

	memcpy_s(data, strlen("Texture:"), "Texture:", strlen("Texture:"));
	//+1 to get the '0' to be there too                            V
	memcpy_s(data + strlen("Texture:"), this->m_source.getSize() + 1, this->m_source.toAnsiString().c_str(), this->m_source.getSize());

	return data;
}

char* Frame::createMemoryChunk(size_t* size)
{

	//Creates data in the format
	//"FRAME:" + float(frametime) + <TEXTURE>

	assert(size != NULL);
	size_t s = 0;

	char* ins = this->m_tex->createMemoryChunk(&s);

	*size += s;

	*size += strlen("FRAME:") + sizeof(float);

	char* data = (char*)malloc(*size + 1);
	if (data == NULL) {
		return NULL;
	}
	memcpy_s(data, *size, "FRAME:", strlen("FRAME:"));
	memcpy_s(data + strlen("FRAME:"), *size, &m_time, sizeof(float));
	memcpy_s(data + strlen("FRAME:") + sizeof(float), *size, ins, s);
	free(ins);


	return data;
}

int Frame::load(XMLReader& data)
{
	bool b = false;
	this->m_time = float(data.asInt(L"duration/", b));
	if (!b) {
		printf("Error: Element duration missing. Defaulting to 1000.f\n");
		this->m_time = 1000.f;
	}
	if (this->m_tex->parseSourceString(data.getValue(L"image/")) < 0) {
		//Error
		printf("Error loading texture!\n");
	}
	auto tmp = data.asFloatRect(L"rect/", b);
	if (!b)
	{
		//We got no rect (= use the complete texture)
		m_tex_rect.left = 0;
		m_tex_rect.top = 0;
		//Just in case: Check whether the unsigned -> signed conversion yields dataloss (should not happen in the next 20 years though)
		assert(m_tex->getTexture().getSize().x <= INT_MAX && m_tex->getTexture().getSize().y <= INT_MAX);
		m_tex_rect.width = int(m_tex->getTexture().getSize().x);
		m_tex_rect.height = int(m_tex->getTexture().getSize().y);
	}
	else
	{
		//Use a very specific part of the texture
		m_tex_rect.top = int(tmp.top);
		m_tex_rect.left = int(tmp.left);
		m_tex_rect.width = int(tmp.width);
		m_tex_rect.height = int(tmp.height);
	}

	//Add actual rect
	return 0;
}

void FadeOutEffect::play(sf::Sprite& target, float time)
{
	//Check if we are within effect bounds
	if (time < m_start)
		return;
	if (time > m_end)
	{
		//Clear the saved image (if not already done
		if (img.getSize().x * img.getSize().y != 16)
		{
			//Reset to the old texture
			auto t = *target.getTexture();
			t.update(img);
			target.setTexture(t);
			//Recreate an undefined image
			img.create(4, 4, sf::Color::Transparent);
		}
		if (m_new_area != nullptr)
		{
			free(m_new_area);
			m_new_area = nullptr;
		}
		return;
	}
	//Use the stepping
	//First, save the old rect
	auto left = target.getTextureRect().left;
	auto top = target.getTextureRect().top;
	auto width = target.getTextureRect().width;
	auto height = target.getTextureRect().height;

	auto fulltexwidth = target.getTexture()->getSize().x;
	auto fulltexheight = target.getTexture()->getSize().y;

	if (m_new_area == nullptr)
	{
		m_new_area = (sf::Uint8*)malloc(width * height * 4);
		if (m_new_area == nullptr)
		{
			printf("[Critical] Failed to allocate %d bytes in %s:%d\n", width * height * 4, __FILE__, __LINE__);
			return;
		}
	}
	//Get the texture from the Graphics Memory to the CPU/RAM
	img = target.getTexture()->copyToImage();
	auto pixels = img.getPixelsPtr();

	//Iterate through the lines
	for (int i = 0; i < height; i++)
	{
		//Copy each line
		memcpy_s(m_new_area + width * i * 4,
			width * (height - i) * 4,
			(pixels + fulltexwidth*(top + i) * 4 + left * 4),
			width * 4);

	}
	//Now we need to change the pixels in the byte array
	for (size_t i = 0; i < size_t(width) * size_t(height); i++)
	{
		//Access the alpha channel (+3) of every pixel (i*4) with the calculated stepping
		m_new_area[i * 4 + 3] = img.getPixel((unsigned int)(i) % width, (unsigned int)(i) / width).a - sf::Uint8(m_stepping*(time - m_start)*255.f);
	}
	//Copy the old texture
	auto to_apply = *target.getTexture();

	//Now, apply the pixels
	to_apply.update(m_new_area, (unsigned int)(width), (unsigned int)(height), (unsigned int)(left), (unsigned int)(top));

	//Set the new texture
	target.setTexture(to_apply);
}

GObjectBase::~GObjectBase()
{
	destroy();
}

void GObjectBase::unload()
{
	notifyAll(GObjectMessages::Unloaded);
}

size_t GObjectBase::getCurrentAnimationIndex() const
{
#ifdef _DEBUG
	return this->m_current_animation;
#else
	return size_t(0);
#endif
}

std::vector<SFG::Pointer<Animation>>* GObjectBase::getAnimationVector()
{
#ifdef _DEBUG
	return &m_animations;
#else
	return nullptr;
#endif
}

void GObjectBase::destroy()
{
	for (auto a : this->m_animations)
	{
		/*if (a != NULL) {
			delete a;
		}*/
	}
	notifyAll(GObjectMessages::Destroyed);
}

void GObjectBase::draw(sf::RenderTarget& target)
{
#ifdef _DEBUG
	assert(m_current_animation < m_animations.size());
#endif
	if (m_current_animation < m_animations.size())
	{
		auto sprite = m_animations[m_current_animation]->getCurrentFrame()->m_tex.getElement();
		assert(sprite != nullptr);

		m_sprite.setTexture(*sprite);

		m_sprite.setTextureRect(m_animations[m_current_animation]->getCurrentFrame()->m_tex_rect);
		target.draw(this->m_sprite);
	}
}

int GObjectBase::update(float dt)
{
	if (m_current_animation < m_animations.size())
	{
		this->m_animations[this->m_current_animation]->updateAnimation(dt, false);
	}
	return 0;
}

int GObjectBase::load(XMLReader& data)
{
	//Definition states that we shall search a subtree called "GObjectBase" in the given reader and parse that one.

	return 0;
}

int GObjectBase::signals_animation(int msg, const SFG::Pointer<MsgPackage>& info)
{
	//Here, the definition is: info must hold an sf::String* pointing to the name of the animation
	//We can now forward this event to our listeners (or handle / ignore it ourselves)
	if (msg >= Animation::AnimationMessages::AnimationMessagesSize)
		return -1;		//We don't tolerate messages outside of the specification
	dynNotifyAll(msg, info);
	return 0;
}

SFG::Pointer<Loadable> GObjectBase::clone()
{
	//Use the copy constructor
	SFG::Pointer<GObjectBase> ret(new GObjectBase(*this));

	//#TODO: Remove warning once verified
	SFG::Util::printLog(SFG::Util::LogMessageType::Development, __FILE__, __LINE__, "Warning: Untested function called");

	return ret;
}

char* GObjectBase::createMemoryChunk(size_t* size)
{
	assert(size != NULL);
	size_t needed_size = 0;
	needed_size += this->m_graphics_name.getSize() + sizeof(m_depth) + sizeof(m_current_animation);
	std::vector<bytearr> arr;
	arr.resize(m_animations.size());
	for (size_t i = 0; i < m_animations.size(); i++) {
		arr[i].data = m_animations[i]->createMemoryChunk(&arr[i].len);
		if (arr[i].data == NULL) {
			//Free if failed
			printf("[Error] Failed to allocate memory in %s:%d\n", __FILE__, __LINE__);
			/*for (size_t ii = 0; ii < i; ii++) {
			free(arr[ii].data);
			}*/
			return NULL;
		}
		needed_size += arr[i].len;
	}
	char* data = (char*)malloc(needed_size);
	if (data == NULL) {
		perror("ALLOCATION ERROR");
		return NULL;
	}
	//#TODO: Not sure if wstring::size() returns size in bytes or words... Check pls.
	memcpy_s(data, needed_size, "GOBJECTBASE:", strlen("GOBJECTBASE:"));
	memcpy_s(data + strlen("GOBJECTBASE:"), needed_size, this->m_graphics_name.toAnsiString().c_str(), this->m_graphics_name.toAnsiString().size() + 1);
	memcpy_s(data + strlen("GOBJECTBASE:") + this->m_graphics_name.toAnsiString().size() + 1, needed_size, &m_depth, sizeof(m_depth));
	size_t s = strlen("GOBJECTBASE:") + this->m_graphics_name.toAnsiString().size() + 1 + sizeof(m_depth);
	for (size_t i = 0; i < m_animations.size(); i++) {
		memcpy_s(data + s, needed_size - s, arr[i].data, arr[i].len);
	}

	//Format: "GOBJECTBASE:" + name + depth + current_animation {<ANIMATION>}*
	*size = needed_size;
	return data;
}

bool GObjectBase::setSize(float x, float y, ResizePolicy policy)
{
	float x_scale = x / m_sprite.getGlobalBounds().width;
	float y_scale = y / m_sprite.getGlobalBounds().height;

	if (policy == ResizePolicy::Scale)
	{
		//Scale (warning: This doesn't reset the scale, but takes the existing scale into account!)
		assert(m_sprite.getTexture()->isRepeated() == true); //Has to be for this to work
		if (m_sprite.getTexture()->isRepeated() == true) return false;
	}
	else if (policy == ResizePolicy::Repeat)
	{
		assert(m_sprite.getTexture()->isRepeated() == false); //Has to be for this to work
		if (m_sprite.getTexture()->isRepeated() == false) return false;
	}
	m_sprite.scale(x_scale, y_scale);
	return true;
}

int GObjectBase::playAnimation(const sf::String& name)
{
	int i = 0;
	for (auto a : m_animations) {
		if (a->m_name == name) {
			m_current_animation = i;
			a->updateAnimation(0.0f, true); //Restart all
			notifyAll(GObjectMessages::AnimationChanged);
			return i;
		}
		i++;
	}
	return -1;
}

int GObjectBase::addAnimation(const SFG::Pointer<Animation>& ptr)
{
	//Add listener
	auto func = std::bind(&GObjectBase::signals_animation, this, std::placeholders::_1, std::placeholders::_2);
	ptr->addListener(this, MsgObjectAction(func));
	//Push into memory
	m_animations.push_back(ptr);
	return 0;
}

int Texture::parseSourceString(const sf::String& str)
{
	//Per definition, the "protocol" header is 7 Bytes long and can be "__file:" or "__data:"
	//Check if already loaded by gov
	for (auto l : *this->m_governor->getData()) {
		if (l.getElement()->m_source == str) {
			//Found existing
			//this->m_texture = static_cast<std::shared_ptr<sf::Texture>>(l); //Copy instruction (SFML should internally still use only 1 image
			m_texture = l;
			this->m_source = str;
			return 1;
		}
	}


	//sf::String prt(str.begin(), str.begin() + 7);
	sf::String prt = str.substring(0, 7);
	if (prt == L"__file:")
	{
		//The standard way: Load a texture from a file
		this->m_source = str;	//This is used to identify identical textures to minimize harddrive access
		//sf::String actualpath(str.begin() + 7, str.end());
		sf::String actualpath = str.substring(7);
		if (!m_texture.isValid())
		{
			//Need to create a new texture
			m_texture.reset(new sf::Texture());
			/*if (m_texture == NULL) {
				printf("[Error] Allocation error in %s:%d\n", __FILE__, __LINE__);
				return -2;
			}*/
		}


		if (!m_texture->loadFromFile(actualpath))
		{
			//Error
			printf("Error loading file\n");
			return -1;
		}
		else
		{
			this->m_source = str;
			return 0;
		}
	}

	return 0;
}

void Animation::updateAnimation(float dt, bool restart)
{
	//dt being logical time
	if (restart) m_anim_state = 0.0f;
	else m_anim_state += dt;

	//If anim_state too big
	while (m_anim_state >= m_totaltime)
		m_anim_state -= m_totaltime;


	float frametime = 0.f;
	for (size_t i = 0; i < m_frames.size(); i++)
	{
		frametime += float(m_frames[i]->m_time);
		if (frametime > m_anim_state)
		{
			if (this->m_current_frame == m_frames.size() - 1 && i == 0)
			{
				SFG::Pointer<MsgPackage> msgpkg(new MsgPackage());

				//Add the name of the animation
				msgpkg->addValue(&this->m_name);
				//Notify all listeners
				dynNotifyAll(AnimationMessages::AnimationFinished, msgpkg);
			}
			this->m_current_frame = i;
			return;
		}
	}

	//updateAnimation(dt); //Again, should not loop. Try if necessary
}

int Animation::load(XMLReader& data)
{
	this->m_xml_source = data;

	//Parse the XMLReader
	m_name = data.getValue(L"name.");

	int anim_width = 0;
	int anim_height = 0;

	sf::String img_path;

	auto w_str = data.getValue(L"width.");
	if (w_str != L"__xml_failure")
	{
		auto h_str = data.getValue(L"height.");
		if (h_str != L"__xml_failure")
		{
			//Get the image tag
			img_path = data.getValue(L"image.");
			if (img_path == L"__xml_failure")
			{
				printf("[Error] Wrong format: When specifying ext. file animation, no image path was provided in %s:%d.\n", __FILE__, __LINE__);
				assert(false); //should not happen
			}
			//We need to specify the frames by ourselves

			//Set the dimensions
			anim_width = std::stoi(w_str.toAnsiString());
			anim_height = std::stoi(h_str.toAnsiString());
			assert(anim_width > 0);
			assert(anim_height > 0);
		}
	}

	bool b = false;
	size_t max_frames = data.asInt(L"frame-count.", b);
	if (!b) {
		max_frames = 10000;	//Should be enough :P
	}

	//Add animation stuff
	size_t i_frame = 0;
	while (i_frame < max_frames) {
		sf::String frames(L"frame#" + std::to_wstring(i_frame) + L"/");
		if (data.getValue(frames) == L"__xml_failure") {
			//Error. Please handle
			break;
		}

		SFG::Pointer<Frame> f(new Frame(reinterpret_cast<Module*>(this)));
		f->m_tex->setGovernor(this->m_governor);
		auto handle = data.getXMLGroupHandle(frames);
		if (handle != nullptr)
		{
			XMLWriter tmpwrt;
			tmpwrt.setBaseGroup(*handle);
			//Create the image group
			XMLGroup img_group;
			img_group.name = L"image";
			img_group.content = L"__file:" + img_path;
			tmpwrt.addGroup(L"", img_group);
			//f->load(XMLReader(*handle));
			XMLReader reader(tmpwrt);
			f->load(reader);
		}
		//If needed, add the rect
		if (anim_width > 0)
		{
			//Set the already known facts
			f->m_tex_rect.width = anim_width;
			f->m_tex_rect.height = anim_height;

			//As the format guarantees that the next frames always are on the right (and other animations in the next row)
			f->m_tex_rect.left = int(i_frame) * anim_width;
			f->m_tex_rect.top = 0; //#TODO: This should be changed when handling mutliple animations
		}

		sf::String image(frames + L"image/");
		if (f->m_tex->parseSourceString(image) < 0) {
			//Error
			printf("Error loading texture!\n");
			assert(false); //Just break
		}
		this->addFrame(f);

		i_frame++;
	}

	return 0;
}

char* Animation::createMemoryChunk(size_t* size)
{
	//Format: "ANIMATION:" + size_t(m_current_frame) + float(m_totaltime) + float(animstate) + {<FRAME>}*
	assert(size != NULL);

	std::vector<bytearr> tmparr;
	tmparr.resize(this->m_frames.size());

	for (size_t i = 0; i < this->m_frames.size(); i++)
	{
		tmparr[i].data = m_frames[i]->createMemoryChunk(&tmparr[i].len);
		if (tmparr[i].data == NULL) {
			//Free if failed
			printf("[Error] Failed to allocate memory in %s:%d\n", __FILE__, __LINE__);
			/*for (size_t ii = 0; ii < i; ii++) {
			free(tmparr[ii].data);
			}*/
			return NULL;
		}
		*size += tmparr[i].len;
	}
	//We got all parts, get it rolling
	*size += strlen("ANIMATION:") + sizeof(m_current_frame) + sizeof(m_totaltime) + sizeof(this->m_anim_state);

	char* data = (char*)malloc(*size);
	if (data == NULL)
	{
		printf("[Error] Failed to allocate memory in %s:%d\n", __FILE__, __LINE__);
		/*for (size_t ii = 0; ii < m_frames.size(); ii++) {
		free(tmparr[ii].data);
		}*/
		return NULL;
	}

	memcpy_s(data, *size, "ANIMATION:", strlen("ANIMATION:"));
	memcpy_s(data + strlen("ANIMATION:"), *size, &m_current_frame, sizeof(m_current_frame));
	memcpy_s(data + strlen("ANIMATION:") + sizeof(m_current_frame), *size, &m_totaltime, sizeof(m_totaltime));
	memcpy_s(data + strlen("ANIMATION:") + sizeof(m_current_frame) + sizeof(m_totaltime), *size, &m_anim_state, sizeof(m_anim_state));
	memcpy_s(data + strlen("ANIMATION:") + sizeof(m_current_frame) + sizeof(m_totaltime) + sizeof(m_anim_state), *size, &m_anim_state, sizeof(m_anim_state));

	size_t s = strlen("ANIMATION:") + sizeof(m_current_frame) + sizeof(m_totaltime) + sizeof(m_anim_state);
	for (size_t i = 0; i < this->m_frames.size(); i++)
	{
		memcpy_s(data + s, *size - s + 1, tmparr[i].data, tmparr[i].len);
		s += tmparr[i].len;
	}
	return data;
}

int GTextBase::load(XMLReader& xmldata)
{
	this->m_xml_source = xmldata;

	m_name = xmldata.getValue(L"name.");

	//Load font
	bool b = false;
	sf::String fontname = xmldata.getValue(L"font/");
	if (!this->m_font.loadFromFile("fonts/" + fontname)) {
		//Error loading font
		printf("Failed to load font %s\n", std::string("fonts/" + fontname).c_str());
	}

	//Load size
	this->m_size = (float)xmldata.asInt(L"size/", b);
	if (!b) {
		printf("Error: Size specifier of font not found. Defaulting to 12\n");
		this->m_size = 12.f;
	}

	//Load language
	sf::String langstr = this->m_governor->getLanguage();
	//Load localized string
	this->m_string = xmldata.getValue(langstr + L"/");


	return 0;
}
