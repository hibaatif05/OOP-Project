#include "Button.h"
Button::Button() {}

Button::Button(const sf::Font& font, const std::string& text,
    sf::Vector2f pos, sf::Vector2f size, Style style)
    : currentStyle(style)
{
    switch (style) {
    case Style::PRIMARY:
    case Style::DANGER:
    case Style::SUCCESS:
    case Style::WARNING:
        normalColor = sf::Color(0, 130, 160);
        hoverColor = sf::Color(0, 100, 125);
        break;
    case Style::GHOST:
        normalColor = sf::Color(255, 255, 255, 18);
        hoverColor = sf::Color(0, 130, 160, 180);
        break;
    }

    // --- Shadow ---
    shadow.setSize({ size.x, size.y });
    shadow.setPosition(pos.x + 3.f, pos.y + 4.f);
    shadow.setFillColor(sf::Color(0, 0, 0, 60));

    // --- Main shape ---
    shape.setPosition(pos);
    shape.setSize(size);
    shape.setFillColor(normalColor);
    shape.setOutlineThickness(1.f);
    if (style == Style::GHOST)
        shape.setOutlineColor(sf::Color(255, 255, 255, 60));
    else
        shape.setOutlineColor(sf::Color(0, 100, 125, 120));

    // --- Left accent strip (GHOST only) ---
    accentStrip.setSize({ 4.f, size.y });
    accentStrip.setPosition(pos.x, pos.y);
    accentStrip.setFillColor(sf::Color(0, 130, 160));

    // --- Label ---
    label.setFont(font);
    label.setString(text);
    label.setCharacterSize(Theme::FS_BODY);
    label.setFillColor(Theme::TEXT_WHITE);

    sf::FloatRect tb = label.getLocalBounds();
    label.setOrigin(tb.left + tb.width / 2.f, tb.top + tb.height / 2.f);
    label.setPosition(pos.x + size.x / 2.f, pos.y + size.y / 2.f);
}

void Button::setPosition(sf::Vector2f pos) {
    shadow.setPosition(pos.x + 3.f, pos.y + 4.f);
    shape.setPosition(pos);
    accentStrip.setPosition(pos.x, pos.y);
    sf::FloatRect tb = label.getLocalBounds();
    label.setOrigin(tb.left + tb.width / 2.f, tb.top + tb.height / 2.f);
    sf::Vector2f sz = shape.getSize();
    label.setPosition(pos.x + sz.x / 2.f, pos.y + sz.y / 2.f);
}

void Button::setSize(sf::Vector2f size) {
    shape.setSize(size);
    shadow.setSize(size);
    accentStrip.setSize({ 4.f, size.y });
    setPosition(shape.getPosition());
}

void Button::setText(const std::string& text, const sf::Font& font) {
    label.setFont(font);
    label.setString(text);
    sf::FloatRect tb = label.getLocalBounds();
    label.setOrigin(tb.left + tb.width / 2.f, tb.top + tb.height / 2.f);
    sf::Vector2f sz = shape.getSize();
    sf::Vector2f pos = shape.getPosition();
    label.setPosition(pos.x + sz.x / 2.f, pos.y + sz.y / 2.f);
}

void Button::setEnabled(bool e) {
    enabled = e;
    shape.setFillColor(e ? normalColor : Theme::TEXT_DARK);
}

bool Button::handleEvent(const sf::Event& e, const sf::RenderWindow& win) {
    if (!enabled) return false;
    sf::Vector2i mp = sf::Mouse::getPosition(win);
    hovered = shape.getGlobalBounds().contains((float)mp.x, (float)mp.y);
    shape.setFillColor(hovered ? hoverColor : normalColor);
    if (e.type == sf::Event::MouseButtonReleased &&
        e.mouseButton.button == sf::Mouse::Left && hovered)
        return true;
    return false;
}

void Button::draw(sf::RenderWindow& win) {
    win.draw(shadow);
    win.draw(shape);
    if (currentStyle == Style::GHOST)
        win.draw(accentStrip);
    win.draw(label);
}

sf::FloatRect Button::getBounds() const { return shape.getGlobalBounds(); }