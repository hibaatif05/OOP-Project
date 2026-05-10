#include "AdminScreen.h"
#include "FileHandler.h"
#include "Validator.h"
#include <fstream>
#include "Utils.h"
#include <sstream>
#include <iomanip>

AdminScreen::AdminScreen(sf::Font& f, Admin* a,
    Storage<Patient>* pats, Storage<Doctor>* docs,
    Storage<Appointment>* appts, Storage<Bill>* b,
    Storage<Prescription>* prescs)
    : font(f), admin(a), patients(pats), doctors(docs),
    appointments(appts), bills(b), prescriptions(prescs) {
    buildLayout();
    setupAllAppointments();
}

//Layout 

void AdminScreen::buildLayout() {
    sidebar.setSize({ 230.f,(float)Theme::WIN_H });
    sidebar.setPosition(0.f, 0.f); sidebar.setFillColor(Theme::SIDEBAR_BG);
    topBar.setSize({ 230.f,4.f }); topBar.setPosition(0.f, 0.f);
    topBar.setFillColor(Theme::ACCENT);
    contentArea.setSize({ (float)Theme::WIN_W - 230.f,(float)Theme::WIN_H });
    contentArea.setPosition(230.f, 0.f); contentArea.setFillColor(Theme::BG);
    listBg.setSize({ (float)Theme::WIN_W - 250.f,(float)Theme::WIN_H - 120.f });
    listBg.setPosition(240.f, 70.f); listBg.setFillColor(Theme::PANEL);

    headerText.setFont(font); headerText.setCharacterSize(Theme::FS_SMALL);
    headerText.setFillColor(Theme::ACCENT);
    headerText.setPosition(14.f, 14.f);
    headerText.setString("MediCore Admin Panel");

    viewTitle.setFont(font); viewTitle.setCharacterSize(Theme::FS_HEAD);
    viewTitle.setFillColor(Theme::TEXT_WHITE); viewTitle.setStyle(sf::Text::Bold);
    viewTitle.setPosition(250.f, 20.f);

    msgText.setFont(font); msgText.setCharacterSize(Theme::FS_SMALL);
    msgText.setPosition(250.f, 686.f);

    float bx = 10.f, bw = 210.f, bh = 38.f, by = 50.f, gap = 4.f;
    btnAddDoc = Button(font, "Add Doctor", { bx, by }, { bw,bh }, Button::Style::SUCCESS);
    btnRemoveDoc = Button(font, "Remove Doctor", { bx, by + (bh + gap) }, { bw,bh }, Button::Style::DANGER);
    btnPatients = Button(font, "All Patients", { bx, by + 2 * (bh + gap) }, { bw,bh }, Button::Style::GHOST);
    btnDoctors = Button(font, "All Doctors", { bx, by + 3 * (bh + gap) }, { bw,bh }, Button::Style::GHOST);
    btnAppointments = Button(font, "All Appointments", { bx, by + 4 * (bh + gap) }, { bw,bh }, Button::Style::GHOST);
    btnUnpaid = Button(font, "Unpaid Bills", { bx, by + 5 * (bh + gap) }, { bw,bh }, Button::Style::WARNING);
    btnDischarge = Button(font, "Discharge Patient", { bx, by + 6 * (bh + gap) }, { bw,bh }, Button::Style::GHOST);
    btnSecLog = Button(font, "Security Log", { bx, by + 7 * (bh + gap) }, { bw,bh }, Button::Style::GHOST);
    btnReport = Button(font, "Daily Report", { bx, by + 8 * (bh + gap) }, { bw,bh }, Button::Style::PRIMARY);
    btnLogout = Button(font, "Logout", { bx, by + 9 * (bh + gap) }, { bw,bh }, Button::Style::DANGER);

    // Form labels
    auto mkLbl = [&](sf::Text& lbl, const std::string& s) {
        lbl.setFont(font); lbl.setString(s);
        lbl.setCharacterSize(Theme::FS_SMALL); lbl.setFillColor(Theme::TEXT_GRAY);
        };
    mkLbl(lName, "Doctor Name");
    mkLbl(lSpec, "Specialization");
    mkLbl(lContact, "Contact (11 digits)");
    mkLbl(lPassword, "Password (min 6)");
    mkLbl(lFee, "Consultation Fee (PKR)");
    mkLbl(lInput1, "");

    tbInput1 = TextBox(font, "", { 250.f,500.f }, { 350.f,Theme::INPUT_H });
    btnConfirm = Button(font, "Confirm", { 250.f,590.f }, { 190.f,Theme::BTN_H }, Button::Style::PRIMARY);
    btnBack2 = Button(font, "Cancel", { 460.f,590.f }, { 190.f,Theme::BTN_H }, Button::Style::GHOST);
}

void AdminScreen::setupAddDoctor() {
    viewTitle.setString("Add Doctor");
    message.clear(); listRows.clear(); colPositions.clear();

    float cx = 250.f, col2 = 640.f, iw = 350.f, iw2 = 300.f, ih = Theme::INPUT_H;
    float row1 = 80.f, row2 = 180.f, row3 = 280.f;

    lName.setPosition(cx, row1);
    tbName = TextBox(font, "e.g. Sara Khan", { cx,   row1 + 22.f }, { iw,  ih });

    lSpec.setPosition(col2, row1);
    tbSpec = TextBox(font, "e.g. Cardiology", { col2, row1 + 22.f }, { iw2, ih });

    lContact.setPosition(cx, row2);
    tbContact = TextBox(font, "03XXXXXXXXX", { cx,   row2 + 22.f }, { iw,  ih });

    lPassword.setPosition(col2, row2);
    tbPassword = TextBox(font, "min 6 chars", { col2, row2 + 22.f }, { iw2, ih }, true);

    lFee.setPosition(cx, row3);
    tbFee = TextBox(font, "e.g. 1500", { cx,   row3 + 22.f }, { iw,  ih });

    btnConfirm.setText("Add Doctor", font);
    btnConfirm.setPosition({ cx, row3 + 90.f });
    btnBack2.setPosition({ cx + 210.f, row3 + 90.f });
}

void AdminScreen::setupRemoveDoctor() {
    viewTitle.setString("Remove Doctor");
    setupAllDoctors();
    viewTitle.setString("Remove Doctor");
    lInput1.setString("Doctor ID to remove");
    lInput1.setPosition(250.f, 500.f);
    tbInput1 = TextBox(font, "Enter Doctor ID", { 250.f,522.f }, { 300.f,Theme::INPUT_H });
    btnConfirm.setText("Remove", font);
    btnConfirm.setPosition({ 250.f,580.f });
    btnBack2.setPosition({ 460.f,580.f });
}

void AdminScreen::setupAllPatients() {
    viewTitle.setString("All Patients");
    listRows.clear(); scrollOffset = 0;
    // Fixed pixel positions for each column
    colPositions = { 248.f, 292.f, 432.f, 467.f, 490.f, 590.f, 672.f };

    listRows.push_back("ID|Name|Age|G|Contact|Balance|Unpaid");
    listRows.push_back(std::string(75, '-'));

    for (int i = 0; i < patients->size(); ++i) {
        const Patient& p = patients->getAt(i);
        int unpaid = 0;
        for (int j = 0; j < bills->size(); ++j)
            if (bills->getAt(j).getPatientId() == p.getId() &&
                bills->getAt(j).getStatus() == "unpaid") ++unpaid;

        std::ostringstream bal;
        bal << std::fixed << std::setprecision(2) << p.getBalance();

        listRows.push_back(
            std::to_string(p.getId()) + "|" +
            p.getName() + "|" +
            std::to_string(p.getAge()) + "|" +
            p.getGender() + "|" +
            p.getContact() + "|" +
            bal.str() + "|" +
            std::to_string(unpaid)
        );
    }
    if (patients->size() == 0) listRows.push_back("No patients found.");
}

void AdminScreen::setupAllDoctors() {
    viewTitle.setString("All Doctors");
    listRows.clear(); scrollOffset = 0;
    // Fixed pixel positions for each column
    colPositions = { 248.f, 292.f, 432.f, 572.f, 672.f };

    listRows.push_back("ID|Name|Specialization|Contact|Fee");
    listRows.push_back(std::string(72, '-'));

    for (int i = 0; i < doctors->size(); ++i) {
        const Doctor& d = doctors->getAt(i);
        std::ostringstream fee;
        fee << std::fixed << std::setprecision(2) << d.getFee();
        listRows.push_back(
            std::to_string(d.getId()) + "|" +
            d.getName() + "|" +
            d.getSpecialization() + "|" +
            d.getContact() + "|" +
            fee.str()
        );
    }
    if (doctors->size() == 0) listRows.push_back("No doctors found.");
}

void AdminScreen::setupAllAppointments() {
    viewTitle.setString("All Appointments");
    listRows.clear(); scrollOffset = 0;
    // Fixed pixel positions for each column
    colPositions = { 248.f, 290.f, 410.f, 515.f, 610.f, 660.f };

    int n = appointments->size();
    int* idx = new int[n];
    for (int i = 0; i < n; ++i) idx[i] = i;
    // Sort descending by date
    for (int i = 1; i < n; ++i) {
        int key = idx[i], j = i - 1;
        while (j >= 0 && dateToInt(appointments->getAt(idx[j]).getDate()) <
            dateToInt(appointments->getAt(key).getDate()))
        {
            idx[j + 1] = idx[j]; --j;
        }
        idx[j + 1] = key;
    }

    listRows.push_back("ID|Patient|Doctor|Date|Slot|Status");
    listRows.push_back(std::string(80, '-'));

    for (int i = 0; i < n; ++i) {
        const Appointment& a = appointments->getAt(idx[i]);
        Patient* p = patients->findById(a.getPatientId());
        Doctor* d = doctors->findById(a.getDoctorId());
        listRows.push_back(
            std::to_string(a.getAppointmentId()) + "|" +
            (p ? p->getName() : "Unknown") + "|" +
            (d ? d->getName() : "Unknown") + "|" +
            a.getDate() + "|" +
            a.getTimeSlot() + "|" +
            a.getStatus()
        );
    }
    if (n == 0) listRows.push_back("No appointments found.");
    delete[] idx;
}

void AdminScreen::setupUnpaidBills() {
    viewTitle.setString("Unpaid Bills");
    listRows.clear(); scrollOffset = 0;
    // Fixed pixel positions for each column
    colPositions = { 248.f, 310.f, 480.f, 600.f };

    std::string today = getTodayDate();
    listRows.push_back("BillID|Patient|Amount (PKR)|Date");
    listRows.push_back(std::string(58, '-'));

    bool any = false;
    for (int i = 0; i < bills->size(); ++i) {
        const Bill& b = bills->getAt(i);
        if (b.getStatus() != "unpaid") continue;
        Patient* p = patients->findById(b.getPatientId());
        std::string dateField = b.getDate();
        if (daysDiff(today, b.getDate()) > 7) dateField += " [OVERDUE]";
        std::ostringstream amt;
        amt << std::fixed << std::setprecision(2) << b.getAmount();
        listRows.push_back(
            std::to_string(b.getBillId()) + "|" +
            (p ? p->getName() : "Unknown") + "|" +
            amt.str() + "|" +
            dateField
        );
        any = true;
    }
    if (!any) listRows.push_back("No unpaid bills.");
}

void AdminScreen::setupDischarge() {
    viewTitle.setString("Discharge Patient");
    setupAllPatients();
    viewTitle.setString("Discharge Patient");
    lInput1.setString("Patient ID to discharge");
    lInput1.setPosition(250.f, 500.f);
    tbInput1 = TextBox(font, "Enter Patient ID", { 250.f,522.f }, { 300.f,Theme::INPUT_H });
    btnConfirm.setText("Discharge", font);
    btnConfirm.setPosition({ 250.f,580.f });
    btnBack2.setPosition({ 460.f,580.f });
}

void AdminScreen::setupSecurityLog() {
    viewTitle.setString("Security Log");
    listRows.clear(); scrollOffset = 0;
    colPositions.clear();   // free-form text, no column splitting
    std::ifstream f("security_log.txt");
    std::string line; bool any = false;
    while (std::getline(f, line)) { if (!line.empty()) { listRows.push_back(line); any = true; } }
    if (!any) listRows.push_back("No security events logged.");
}

void AdminScreen::setupDailyReport() {
    viewTitle.setString("Daily Report");
    listRows.clear(); scrollOffset = 0;
    colPositions = { 248.f, 430.f, 530.f, 615.f };

    std::string today = getTodayDate();
    listRows.push_back("=== Daily Report for " + today + " ===");
    listRows.push_back("");

    int total = 0, pending = 0, completed = 0, noshow = 0, cancelled = 0;
    for (int i = 0; i < appointments->size(); ++i) {
        const Appointment& a = appointments->getAt(i);
        if (a.getDate() != today) continue;
        ++total;
        if (a.getStatus() == "pending")   ++pending;
        else if (a.getStatus() == "completed") ++completed;
        else if (a.getStatus() == "noshow")    ++noshow;
        else if (a.getStatus() == "cancelled") ++cancelled;
    }
    std::ostringstream apptLine;
    apptLine << "Total appointments today: " << total
        << "  (Pending: " << pending << "  Completed: " << completed
        << "  No-show: " << noshow << "  Cancelled: " << cancelled << ")";
    listRows.push_back(apptLine.str());
    listRows.push_back("");

    float revenue = 0.f;
    for (int i = 0; i < bills->size(); ++i) {
        const Bill& b = bills->getAt(i);
        if (b.getStatus() == "paid" && b.getDate() == today) revenue += b.getAmount();
    }
    std::ostringstream revLine;
    revLine << "Revenue today (paid bills): PKR "
        << std::fixed << std::setprecision(2) << revenue;
    listRows.push_back(revLine.str());
    listRows.push_back("");

    // --- Unpaid bills section ---
    listRows.push_back("Patients with outstanding unpaid bills:");
    listRows.push_back(std::string(50, '-'));

    // Use colPositions {248, 430} for two-column pipe rows
    for (int i = 0; i < patients->size(); ++i) {
        const Patient& p = patients->getAt(i);
        float owed = 0.f;
        for (int j = 0; j < bills->size(); ++j)
            if (bills->getAt(j).getPatientId() == p.getId() &&
                bills->getAt(j).getStatus() == "unpaid")
                owed += bills->getAt(j).getAmount();
        if (owed > 0.f) {
            std::ostringstream ss;
            ss << p.getName() << "|PKR "
                << std::fixed << std::setprecision(2) << owed;
            listRows.push_back(ss.str());
        }
    }

    listRows.push_back("");

    // --- Doctor-wise summary section ---
    listRows.push_back("Doctor-wise summary for today:");
    listRows.push_back(std::string(50, '-'));

    // Header uses '|' rendered at colPositions pixel stops
    listRows.push_back("Name|Completed|Pending|No-show");

    for (int i = 0; i < doctors->size(); ++i) {
        const Doctor& d = doctors->getAt(i);
        int dc = 0, dp = 0, dn = 0;
        for (int j = 0; j < appointments->size(); ++j) {
            const Appointment& a = appointments->getAt(j);
            if (a.getDoctorId() != d.getId() || a.getDate() != today) continue;
            if (a.getStatus() == "completed") ++dc;
            else if (a.getStatus() == "pending")   ++dp;
            else if (a.getStatus() == "noshow")    ++dn;
        }
        if (dc + dp + dn > 0) {
            listRows.push_back(
                d.getName() + "|" +
                std::to_string(dc) + "|" +
                std::to_string(dp) + "|" +
                std::to_string(dn)
            );
        }
    }
}

// Actions 

void AdminScreen::doAddDoctor() {
    std::string name = tbName.getValue(),
        spec = tbSpec.getValue(),
        contact = tbContact.getValue(),
        pwd = tbPassword.getValue(),
        feeStr = tbFee.getValue();

    if (name.empty() || spec.empty()) { message = "Name and specialization required."; msgSuccess = false; return; }
    if (!Validator::isValidContact(contact)) { message = "Contact must be 11 digits."; msgSuccess = false; return; }
    if (!Validator::isValidPassword(pwd)) { message = "Password min 6 chars.";       msgSuccess = false; return; }
    if (!Validator::isValidPositiveFloat(feeStr)) { message = "Enter a valid fee.";           msgSuccess = false; return; }

    float fee = std::stof(feeStr);
    int newId = 1;
    for (int i = 0; i < doctors->size(); ++i)
        if (doctors->getAt(i).getId() >= newId) newId = doctors->getAt(i).getId() + 1;

    Doctor d(newId, name, spec, contact, pwd, fee);
    doctors->add(d); FileHandler::appendDoctor(d);
    message = "Doctor added. ID: " + std::to_string(newId); msgSuccess = true;
    tbName.clear(); tbSpec.clear(); tbContact.clear(); tbPassword.clear(); tbFee.clear();
}

void AdminScreen::doRemoveDoctor() {
    std::string inp = tbInput1.getValue(); int docId = 0;
    try { docId = std::stoi(inp); }
    catch (...) { message = "Invalid ID."; msgSuccess = false; return; }
    Doctor* doc = doctors->findById(docId);
    if (!doc) { message = "Doctor not found."; msgSuccess = false; return; }
    for (int i = 0; i < appointments->size(); ++i) {
        const Appointment& a = appointments->getAt(i);
        if (a.getDoctorId() == docId && a.getStatus() == "pending") {
            message = "Cannot remove: has pending appointments."; msgSuccess = false; return;
        }
    }
    doctors->removeById(docId); FileHandler::saveAllDoctors(*doctors);
    message = "Doctor removed."; msgSuccess = true; tbInput1.clear();
    setupRemoveDoctor();
}

void AdminScreen::doDischarge() {
    std::string inp = tbInput1.getValue(); int patId = 0;
    try { patId = std::stoi(inp); }
    catch (...) { message = "Invalid ID."; msgSuccess = false; return; }
    Patient* pat = patients->findById(patId);
    if (!pat) { message = "Patient not found."; msgSuccess = false; return; }
    for (int i = 0; i < bills->size(); ++i)
        if (bills->getAt(i).getPatientId() == patId &&
            bills->getAt(i).getStatus() == "unpaid")
        {
            message = "Cannot discharge: unpaid bills exist."; msgSuccess = false; return;
        }
    for (int i = 0; i < appointments->size(); ++i)
        if (appointments->getAt(i).getPatientId() == patId &&
            appointments->getAt(i).getStatus() == "pending")
        {
            message = "Cannot discharge: pending appointments exist."; msgSuccess = false; return;
        }

    FileHandler::archivePatient(*pat, *appointments, *bills, *prescriptions);
    int pid = pat->getId();
    patients->removeById(pid); FileHandler::saveAllPatients(*patients);
    for (int i = appointments->size() - 1; i >= 0; --i)
        if (appointments->getAt(i).getPatientId() == pid) appointments->removeAt(i);
    FileHandler::saveAllAppointments(*appointments);
    for (int i = bills->size() - 1; i >= 0; --i)
        if (bills->getAt(i).getPatientId() == pid) bills->removeAt(i);
    FileHandler::saveAllBills(*bills);
    for (int i = prescriptions->size() - 1; i >= 0; --i)
        if (prescriptions->getAt(i).getPatientId() == pid) prescriptions->removeAt(i);
    FileHandler::saveAllPrescriptions(*prescriptions);

    message = "Patient discharged and archived."; msgSuccess = true; tbInput1.clear();
    setupDischarge();
}

// Draw list 

void AdminScreen::drawList(sf::RenderWindow& win) {
    win.draw(listBg);
    float y = 78.f, lineH = 22.f;
    int start = scrollOffset;
    int maxLines = (int)((Theme::WIN_H - 120.f) / lineH);

    sf::Text cell;
    cell.setFont(font);
    cell.setCharacterSize(14);

    for (int i = start; i < (int)listRows.size() && i < start + maxLines; ++i) {
        // Alternating row background (skip header row 0 and separator row 1)
        if (i > 1) {
            sf::RectangleShape bg({ (float)Theme::WIN_W - 250.f, lineH });
            bg.setPosition(240.f, y - 2.f);
            bg.setFillColor((i % 2 == 0) ? Theme::ROW_EVEN : Theme::ROW_ODD);
            win.draw(bg);
        }

        const std::string& line = listRows[i];
        bool hasColumns = !colPositions.empty() && (line.find('|') != std::string::npos);

        if (hasColumns) {
            // Split by '|' and render each token at its fixed pixel column x-position
            std::istringstream ss(line);
            std::string token;
            int col = 0;
            while (std::getline(ss, token, '|') && col < (int)colPositions.size()) {
                if (i == 0) {
                    // Header row: accent colour + bold
                    cell.setFillColor(Theme::ACCENT);
                    cell.setStyle(sf::Text::Bold);
                }
                else {
                    cell.setFillColor(Theme::TEXT_WHITE);
                    cell.setStyle(sf::Text::Regular);
                }
                cell.setString(token);
                cell.setPosition(colPositions[col], y);
                win.draw(cell);
                ++col;
            }
        }
        else {
            // Separator lines, free-form report text, or plain messages
            cell.setStyle(i == 0 ? sf::Text::Bold : sf::Text::Regular);
            cell.setFillColor(i == 0 ? Theme::ACCENT : Theme::TEXT_WHITE);
            cell.setString(line);
            cell.setPosition(248.f, y);
            win.draw(cell);
        }

        y += lineH;
    }
}

// Event / Update / Draw 

void AdminScreen::handleEvent(const sf::Event& e, sf::RenderWindow& win) {
    if (btnAddDoc.handleEvent(e, win)) { currentView = View::ADD_DOCTOR;       setupAddDoctor();       return; }
    if (btnRemoveDoc.handleEvent(e, win)) { currentView = View::REMOVE_DOCTOR;    setupRemoveDoctor();    return; }
    if (btnPatients.handleEvent(e, win)) { currentView = View::ALL_PATIENTS;     setupAllPatients();     return; }
    if (btnDoctors.handleEvent(e, win)) { currentView = View::ALL_DOCTORS;      setupAllDoctors();      return; }
    if (btnAppointments.handleEvent(e, win)) { currentView = View::ALL_APPOINTMENTS; setupAllAppointments(); return; }
    if (btnUnpaid.handleEvent(e, win)) { currentView = View::UNPAID_BILLS;     setupUnpaidBills();     return; }
    if (btnDischarge.handleEvent(e, win)) { currentView = View::DISCHARGE;        setupDischarge();       return; }
    if (btnSecLog.handleEvent(e, win)) { currentView = View::SECURITY_LOG;     setupSecurityLog();     return; }
    if (btnReport.handleEvent(e, win)) { currentView = View::DAILY_REPORT;     setupDailyReport();     return; }
    if (btnLogout.handleEvent(e, win)) { nextScreen = ScreenID::MAIN_MENU;                            return; }

    if (e.type == sf::Event::MouseWheelScrolled) {
        scrollOffset -= (int)e.mouseWheelScroll.delta * 2;
        if (scrollOffset < 0) scrollOffset = 0;
    }

    if (currentView == View::ADD_DOCTOR) {
        tbName.handleEvent(e, win);     tbSpec.handleEvent(e, win);
        tbContact.handleEvent(e, win);  tbPassword.handleEvent(e, win);
        tbFee.handleEvent(e, win);
        if (btnConfirm.handleEvent(e, win)) doAddDoctor();
        if (btnBack2.handleEvent(e, win)) { currentView = View::MENU; message.clear(); setupAllAppointments(); }
    }
    else if (currentView == View::REMOVE_DOCTOR) {
        tbInput1.handleEvent(e, win);
        if (btnConfirm.handleEvent(e, win)) doRemoveDoctor();
        if (btnBack2.handleEvent(e, win)) { currentView = View::MENU; message.clear(); setupAllAppointments(); }
    }
    else if (currentView == View::DISCHARGE) {
        tbInput1.handleEvent(e, win);
        if (btnConfirm.handleEvent(e, win)) doDischarge();
        if (btnBack2.handleEvent(e, win)) { currentView = View::MENU; message.clear(); setupAllAppointments(); }
    }
}

void AdminScreen::update(float dt) {
    tbName.update(dt);    tbSpec.update(dt);    tbContact.update(dt);
    tbPassword.update(dt); tbFee.update(dt);    tbInput1.update(dt);
    msgText.setString(message);
    msgText.setFillColor(msgSuccess ? Theme::SUCCESS : Theme::DANGER);
}

void AdminScreen::draw(sf::RenderWindow& win) {
    win.draw(contentArea); win.draw(sidebar); win.draw(topBar);
    win.draw(headerText);

    btnAddDoc.draw(win);     btnRemoveDoc.draw(win);  btnPatients.draw(win);
    btnDoctors.draw(win);    btnAppointments.draw(win); btnUnpaid.draw(win);
    btnDischarge.draw(win);  btnSecLog.draw(win);     btnReport.draw(win);
    btnLogout.draw(win);

    win.draw(viewTitle);

    if (!listRows.empty()) drawList(win);

    if (currentView == View::ADD_DOCTOR) {
        win.draw(lName);     tbName.draw(win);
        win.draw(lSpec);     tbSpec.draw(win);
        win.draw(lContact);  tbContact.draw(win);
        win.draw(lPassword); tbPassword.draw(win);
        win.draw(lFee);      tbFee.draw(win);
        btnConfirm.draw(win); btnBack2.draw(win);
    }
    else if (currentView == View::REMOVE_DOCTOR ||
        currentView == View::DISCHARGE) {
        win.draw(lInput1); tbInput1.draw(win);
        btnConfirm.draw(win); btnBack2.draw(win);
    }

    win.draw(msgText);
}