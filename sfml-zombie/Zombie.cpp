#include "stdafx.h"
#include "Zombie.h"
#include "Player.h"

Zombie::Zombie(const std::string& name)
	: GameObject(name)
{
}

void Zombie::SetPosition(const sf::Vector2f& pos)
{
	GameObject::SetPosition(pos);
	body.setPosition(pos);
}

void Zombie::SetRotation(float rot)
{
	GameObject::SetRotation(rot);
	body.setRotation(rot);
}

void Zombie::SetScale(const sf::Vector2f& s)
{
	GameObject::SetScale(s);
	body.setScale(s);
}

void Zombie::SetOrigin(const sf::Vector2f& o)
{
	GameObject::SetOrigin(o);
	body.setOrigin(o);
}

void Zombie::SetOrigin(Origins preset)
{
	GameObject::SetOrigin(preset);
	if (preset != Origins::Custom)
	{
		Utils::SetOrigin(body, preset);
	}
}

void Zombie::Init()
{
	sortingLayer = SortingLayers::Foreground;
	sortingOrder = 0;

	SetType(type);
}

void Zombie::Release()
{
}

void Zombie::Reset()
{
	player = (Player*) SCENE_MGR.GetCurrentScene()->FindGameObject("Player");

	body.setTexture(TEXTURE_MGR.Get(texId), true);
	SetOrigin(Origins::MC);
	SetPosition({ 0.f,0.f });
	SetRotation(0.f);
	SetScale({ 1.f,1.f });

	hp = maxHp;
	attackTimer = 0.f;
}

void Zombie::Update(float dt)
{
	direction = Utils::GetNormal(player->GetPosition() - position);
	SetRotation(Utils::Angle(direction));
	SetPosition(GetPosition() + speed * dt * direction);

	hitBox.UpdateTransform(body, GetLocalBound());

	attackTimer += dt;
	if (attackTimer > attackInterval)
	{
		if (Utils::CheckCollision(hitBox.rect, player->GetHitBox().rect))
		{
			player->OnDamage(damage);
			attackTimer = 0.f;
		}
	}
}

void Zombie::Draw(sf::RenderWindow& window)
{
	window.draw(body);
	hitBox.Draw(window);
}

void Zombie::SetType(Types type)
{
	this->type = type;
	switch (type)
	{
	case Zombie::Types::Bloater:
		texId = "graphics/bloater.png";
		maxHp = 200;
		speed = 50.f;
		damage = 100;
		attackInterval = 1.f;
		break;
	case Zombie::Types::Chase:
		texId = "graphics/chaser.png";
		maxHp = 100;
		speed = 100.f;
		damage = 100;
		attackInterval = 1.f;
		break;
	case Zombie::Types::Crawler:
		texId = "graphics/crawler.png";
		maxHp = 50;
		speed = 200.f;
		damage = 100;
		attackInterval = 1.f;
		break;
	default:
		break;
	}
}

void Zombie::OnDamage(int damage)
{
	hp = Utils::Clamp(hp - damage, 0, maxHp);
	if (hp == 0)
	{
		SetActive(false);
	}
}
