#include "TextBox.h"
TextBox::TextBox() {}
TextBox::TextBox(const sf::Font& font, const std::string& placeholder,
    sf::Vector2f pos, sf::Vector2f size, bool isPassword)
    : password(isPassword) {

    shape.setPosition(pos);
    shape.setSize(size);
    shape.setFillColor(Theme::INPUT_BG);
    shape.setOutlineThickness(1.f);
    shape.setOutlineColor(Theme::BORDER);

    cursor.setSize({ 2.f, size.y - 16.f });
    cursor.setFillColor(Theme::ACCENT);

    displayText.setFont(font);
    displayText.setCharacterSize(Theme::FS_BODY);
    displayText.setFillColor(Theme::TEXT_WHITE);
    displayText.setPosition(pos.x + 12.f, pos.y + (size.y - 22.f) / 2.f);

    placeholderText.setFont(font);
    placeholderText.setCharacterSize(Theme::FS_BODY);
    placeholderText.setFillColor(Theme::TEXT_DARK);
    placeholderText.setString(placeholder);
    placeholderText.setPosition(pos.x + 12.f, pos.y + (size.y - 22.f) / 2.f);
}

void TextBox::setPosition(sf::Vector2f pos) {
    shape.setPosition(pos);
    sf::Vector2f sz = shape.getSize();
    displayText.setPosition(pos.x + 12.f, pos.y + (sz.y - 22.f) / 2.f);
    placeholderText.setPosition(pos.x + 12.f, pos.y + (sz.y - 22.f) / 2.f);
}

void TextBox::setActive(bool a) {
    active = a;
    shape.setFillColor(a ? Theme::INPUT_ACTIVE : Theme::INPUT_BG);
    shape.setOutlineColor(a ? Theme::ACCENT : Theme::BORDER);
}

bool TextBox::isActive() const { return active; }

const std::string& TextBox::getValue() const { return value; }

void TextBox::setValue(const std::string& v) {
    value = v;
    refreshDisplay();
}

void TextBox::clear() {
    value.clear();
    refreshDisplay();
}

void TextBox::refreshDisplay() {
    if (password)
        displayText.setString(std::string(value.size(), '*'));
    else
        displayText.setString(value);
}

bool TextBox::handleEvent(const sf::Event& e, const sf::RenderWindow& win) {
    if (e.type == sf::Event::MouseButtonPressed &&
        e.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2i mp = sf::Mouse::getPosition(win);
        bool inside = shape.getGlobalBounds().contains((float)mp.x, (float)mp.y);
        setActive(inside);
    }

    if (!active) return false;

    if (e.type == sf::Event::TextEntered) {
        uint32_t c = e.text.unicode;
        if (c == '\b') {                    
            if (!value.empty()) {
                value.pop_back();
                refreshDisplay();
            }
        }
        else if (c == '\r' || c == '\n') { 
            return true;
        }
        else if (c >= 32 && c < 127) {    
            if (value.size() < 100) {
                value += static_cast<char>(c);
                refreshDisplay();
            }
        }
    }
    return false;
}

void TextBox::update(float dt) {
    cursorTimer += dt;
    if (cursorTimer >= 0.5f) { cursorTimer = 0.f; cursorVis = !cursorVis; }
}

void TextBox::draw(sf::RenderWindow& win) {
    win.draw(shape);
    if (value.empty() && !active)
        win.draw(placeholderText);
    else
        win.draw(displayText);
    if (active && cursorVis) {
        sf::FloatRect tb = displayText.getGlobalBounds();
        sf::Vector2f  sp = shape.getPosition();
        sf::Vector2f  ss = shape.getSize();
        float cx = value.empty() ? sp.x + 12.f : tb.left + tb.width + 2.f;
        float cy = sp.y + 8.f;
        cursor.setPosition(cx, cy);
        win.draw(cursor);
    }
}
