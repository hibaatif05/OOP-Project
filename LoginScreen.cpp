#include "LoginScreen.h"
#include "Theme.h"
#include "Utils.h"
#include <ctime>
#include <string>

static void buildCard(sf::RectangleShape& card, sf::RectangleShape& bar,
    sf::Text& title, sf::Text& idLbl, sf::Text& pwLbl,
    TextBox& tbId, TextBox& tbPw,
    Button& btnLogin, Button& btnBack,
    sf::Text& errText, sf::Text& attText,
    const std::string& roleTitle, sf::Font& font) {
    card.setSize({ 480.f, 420.f });
    card.setPosition((Theme::WIN_W - 480.f) / 2.f, (Theme::WIN_H - 420.f) / 2.f);
    card.setFillColor(Theme::PANEL);

    bar.setSize({ 480.f, 5.f });
    bar.setPosition(card.getPosition().x, card.getPosition().y);
    bar.setFillColor(Theme::ACCENT);

    float cx = card.getPosition().x;
    float cy = card.getPosition().y;

    title.setFont(font);
    title.setString(roleTitle + " Login");
    title.setCharacterSize(Theme::FS_HEAD);
    title.setFillColor(Theme::TEXT_WHITE);
    title.setStyle(sf::Text::Bold);
    sf::FloatRect tb = title.getLocalBounds();
    title.setPosition(cx + (480.f - tb.width) / 2.f, cy + 24.f);

    idLbl.setFont(font);  idLbl.setString("ID");
    idLbl.setCharacterSize(Theme::FS_SMALL); idLbl.setFillColor(Theme::TEXT_GRAY);
    idLbl.setPosition(cx + 40.f, cy + 90.f);

    tbId = TextBox(font, "Enter your ID", { cx + 40.f, cy + 112.f }, { 400.f, Theme::INPUT_H });

    pwLbl.setFont(font); pwLbl.setString("Password");
    pwLbl.setCharacterSize(Theme::FS_SMALL); pwLbl.setFillColor(Theme::TEXT_GRAY);
    pwLbl.setPosition(cx + 40.f, cy + 180.f);

    tbPw = TextBox(font, "Enter password", { cx + 40.f, cy + 202.f }, { 400.f, Theme::INPUT_H }, true);

    btnLogin = Button(font, "Login", { cx + 40.f,  cy + 270.f }, { 185.f, Theme::BTN_H }, Button::Style::PRIMARY);
    btnBack = Button(font, "Back", { cx + 255.f, cy + 270.f }, { 185.f, Theme::BTN_H }, Button::Style::GHOST);

    errText.setFont(font); errText.setCharacterSize(Theme::FS_SMALL);
    errText.setFillColor(Theme::DANGER); errText.setPosition(cx + 40.f, cy + 332.f);

    attText.setFont(font); attText.setCharacterSize(Theme::FS_SMALL);
    attText.setFillColor(Theme::WARNING); attText.setPosition(cx + 40.f, cy + 358.f);
}

//Patient constructor 
LoginScreen::LoginScreen(sf::Font& f, Storage<Patient>& pats, SuccessCallback cb)
    : font(f), returnScreen(ScreenID::MAIN_MENU),
    successScreen(ScreenID::PATIENT_MENU),
    patients(&pats), role("Patient"), onSuccess(cb) {
    buildCard(card, topBar, titleText, idLabel, pwLabel,
        tbId, tbPassword, btnLogin, btnBack,
        errorText, attemptsText, role, font);
}

//Doctor constructor
LoginScreen::LoginScreen(sf::Font& f, Storage<Doctor>& docs, SuccessCallback cb)
    : font(f), returnScreen(ScreenID::MAIN_MENU),
    successScreen(ScreenID::DOCTOR_MENU),
    doctors(&docs), role("Doctor"), onSuccess(cb) {
    buildCard(card, topBar, titleText, idLabel, pwLabel,
        tbId, tbPassword, btnLogin, btnBack,
        errorText, attemptsText, role, font);
}

//Admin constructor
LoginScreen::LoginScreen(sf::Font& f, Admin& adm, SuccessCallback cb)
    : font(f), returnScreen(ScreenID::MAIN_MENU),
    successScreen(ScreenID::ADMIN_MENU),
    admin(&adm), role("Admin"), onSuccess(cb) {
    buildCard(card, topBar, titleText, idLabel, pwLabel,
        tbId, tbPassword, btnLogin, btnBack,
        errorText, attemptsText, role, font);
}
void LoginScreen::logFailed(const std::string& enteredId) {
    time_t t = time(nullptr);
    char buf[30];
    struct tm timeInfo;
    localtime_s(&timeInfo, &t);
    strftime(buf, sizeof(buf), "%d-%m-%Y %H:%M:%S", &timeInfo);
    FileHandler::appendSecurityLog(std::string(buf), role, enteredId, "FAILED");
}

bool LoginScreen::tryLogin() {
    std::string idStr = tbId.getValue();
    std::string pwd = tbPassword.getValue();
    if (idStr.empty() || pwd.empty()) { errorMsg = "Please fill in both fields."; return false; }

    try {
        int id = std::stoi(idStr);
        if (role == "Patient" && patients) {
            Patient* p = patients->findById(id);
            if (p && p->getPassword() == pwd) { if (onSuccess) onSuccess(id); return true; }
        }
        else if (role == "Doctor" && doctors) {
            Doctor* d = doctors->findById(id);
            if (d && d->getPassword() == pwd) { if (onSuccess) onSuccess(id); return true; }
        }
        else if (role == "Admin" && admin) {
            if (admin->getId() == id && admin->getPassword() == pwd) {
                if (onSuccess) onSuccess(id); return true;
            }
        }
    }
    catch (...) {}
    return false;
}
void LoginScreen::handleEvent(const sf::Event& e, sf::RenderWindow& win) {
    if (locked) {
        if (btnBack.handleEvent(e, win)) {
            nextScreen = returnScreen;
            locked = false;
            failedAttempts = 0;
            errorMsg.clear();
            tbId.clear();
            tbPassword.clear();
        }
        return;
    }

    tbId.handleEvent(e, win);
    tbPassword.handleEvent(e, win);

    bool doLogin = btnLogin.handleEvent(e, win);

    // Also allow Enter to submit
    if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Return)
        doLogin = true;

    if (doLogin) {
        if (tryLogin()) {
            nextScreen = successScreen;
            failedAttempts = 0; errorMsg.clear();
            tbId.clear(); tbPassword.clear();
        }
        else {
            ++failedAttempts;
            logFailed(tbId.getValue());
            tbPassword.clear();
            if (failedAttempts >= 3) {
                locked = true;
                errorMsg = "Account locked. Contact admin.";
            }
            else {
                errorMsg = "Invalid credentials. " +
                    std::to_string(3 - failedAttempts) + " attempt(s) remaining.";
            }
        }
    }

    if (btnBack.handleEvent(e, win)) {
        nextScreen = returnScreen;
        failedAttempts = 0; errorMsg.clear();
        tbId.clear(); tbPassword.clear();
    }
}

void LoginScreen::update(float dt) {
    tbId.update(dt);
    tbPassword.update(dt);

    errorText.setString(errorMsg);
    if (failedAttempts > 0 && !locked)
        attemptsText.setString("Failed attempts: " + std::to_string(failedAttempts) + " / 3");
    else
        attemptsText.setString("");
}
void LoginScreen::draw(sf::RenderWindow& win) {
    win.draw(card);
    win.draw(topBar);
    win.draw(titleText);
    win.draw(idLabel);
    win.draw(pwLabel);
    tbId.draw(win);
    tbPassword.draw(win);
    btnLogin.draw(win);
    btnBack.draw(win);
    win.draw(errorText);
    win.draw(attemptsText);
}
