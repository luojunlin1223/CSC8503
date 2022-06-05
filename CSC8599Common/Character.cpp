#include "pch.h"
#include "Character.h"
#include "../Common/Assets.h"
NCL::CSC8599::Character::Character()
{
	initAttrs();
}
void NCL::CSC8599::Character::Update(float dt)
{
}
void NCL::CSC8599::Character::initAttrs()
{
	std::ifstream t(NCL::Assets::DATADIR + "character.json");
	rapidjson::IStreamWrapper isw(t);
	rapidjson::Document document;
	document.ParseStream(isw);
	for (rapidjson::Document::ConstMemberIterator itr = document.MemberBegin(); itr != document.MemberEnd(); ++itr)
	{
		if (itr->value.IsString())Attrs[itr->name.GetString()]._string = itr->value.GetString();
		if (itr->value.IsInt())Attrs[itr->name.GetString()]._int = itr->value.GetInt();
		if (itr->value.IsFloat())Attrs[itr->name.GetString()]._float = itr->value.GetFloat();
		if (itr->value.IsBool())Attrs[itr->name.GetString()]._bool = itr->value.GetBool();
	}
	int i;
}