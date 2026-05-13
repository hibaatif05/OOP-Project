#include "PatientScreen.h"
#include "FileHandler.h"
#include "Validator.h"
#include "Utils.h"
#include "Exceptions.h"
#include <sstream>
#include <iomanip>
#include <ctime>

static const std::string SLOTS[] = {
    "09:00","10:00","11:00","12:00","13:00","14:00","15:00","16:00"
};
static const int SLOT_COUNT = 8;

PatientScreen::PatientScreen(sf::Font& f, Patient* p,
    Storage<Patient>* pats, Storage<Doctor>* docs,
    Storage<Appointment>* appts, Storage<Bill>* b,
    Storage<Prescription>* prescs)
    : font(f), patient(p), patients(pats), doctors(docs),
    appointments(appts), bills(b), prescriptions(prescs) {
    buildSidebar();
    buildContentArea();
    refreshHeader();
    setupViewAppts();
    currentView = View::MENU;
}

void PatientScreen::buildSidebar() {
    sidebar.setSize({ 220.f, (float)Theme::WIN_H });
    sidebar.setPosition(0.f, 0.f);
    sidebar.setFillColor(Theme::SIDEBAR_BG);

    topBar.setSize({ 220.f, 4.f });
    topBar.setPosition(0.f, 0.f);
    topBar.setFillColor(Theme::ACCENT);

    headerName.setFont(font); headerName.setCharacterSize(Theme::FS_SMALL);
    headerName.setFillColor(Theme::TEXT_WHITE); headerName.setPosition(14.f, 18.f);

    headerBalance.setFont(font); headerBalance.setCharacterSize(Theme::FS_SMALL);
    headerBalance.setFillColor(Theme::SUCCESS); headerBalance.setPosition(14.f, 40.f);

    float bx = 10.f, bw = 200.f, bh = 40.f, by = 80.f, gap = 6.f;
    btnBook = Button(font, "Book Appointment", { bx,by }, { bw,bh }, Button::Style::PRIMARY);
    btnCancel = Button(font, "Cancel Appointment", { bx,by + (bh + gap) }, { bw,bh }, Button::Style::GHOST);
    btnViewAppts = Button(font, "My Appointments", { bx,by + 2 * (bh + gap) }, { bw,bh }, Button::Style::GHOST);
    btnRecords = Button(font, "Medical Records", { bx,by + 3 * (bh + gap) }, { bw,bh }, Button::Style::GHOST);
    btnBills = Button(font, "View Bills", { bx,by + 4 * (bh + gap) }, { bw,bh }, Button::Style::GHOST);
    btnPayBill = Button(font, "Pay Bill", { bx,by + 5 * (bh + gap) }, { bw,bh }, Button::Style::GHOST);
    btnTopUp = Button(font, "Top Up Balance", { bx,by + 6 * (bh + gap) }, { bw,bh }, Button::Style::SUCCESS);
    btnLogout = Button(font, "Logout", { bx,by + 7 * (bh + gap) }, { bw,bh }, Button::Style::DANGER);
}

void PatientScreen::buildContentArea() {
    contentArea.setSize({ (float)Theme::WIN_W - 220.f, (float)Theme::WIN_H });
    contentArea.setPosition(220.f, 0.f);
    contentArea.setFillColor(Theme::BG);

    viewTitle.setFont(font); viewTitle.setCharacterSize(Theme::FS_HEAD);
    viewTitle.setFillColor(Theme::TEXT_WHITE); viewTitle.setStyle(sf::Text::Bold);
    viewTitle.setPosition(240.f, 20.f);

    msgText.setFont(font); msgText.setCharacterSize(Theme::FS_SMALL);
    msgText.setPosition(240.f, 630.f);

    listBg.setSize({ (float)Theme::WIN_W - 240.f, (float)Theme::WIN_H - 120.f });
    listBg.setPosition(230.f, 70.f);
    listBg.setFillColor(Theme::PANEL);

    lInput1.setFont(font); lInput1.setCharacterSize(Theme::FS_SMALL);
    lInput1.setFillColor(Theme::TEXT_GRAY);
    lInput2.setFont(font); lInput2.setCharacterSize(Theme::FS_SMALL);
    lInput2.setFillColor(Theme::TEXT_GRAY);
    lInput3.setFont(font); lInput3.setCharacterSize(Theme::FS_SMALL);
    lInput3.setFillColor(Theme::TEXT_GRAY);

    tbInput1 = TextBox(font, "", { 240.f, 120.f }, { 400.f, Theme::INPUT_H });
    tbInput2 = TextBox(font, "", { 240.f, 210.f }, { 400.f, Theme::INPUT_H });
    tbInput3 = TextBox(font, "", { 240.f, 300.f }, { 400.f, Theme::INPUT_H });

    btnConfirm = Button(font, "Confirm", { 240.f, 390.f }, { 190.f, Theme::BTN_H }, Button::Style::PRIMARY);
    btnBack2 = Button(font, "Cancel", { 450.f, 390.f }, { 190.f, Theme::BTN_H }, Button::Style::GHOST);
}

void PatientScreen::refreshHeader() {
    headerName.setString("Patient: " + patient->getName());
    std::ostringstream ss;
    ss << "Balance: PKR " << std::fixed << std::setprecision(2) << patient->getBalance();
    headerBalance.setString(ss.str());
}

bool PatientScreen::isSlotTaken(int docId, const std::string& date, const std::string& slot) {
    for (int i = 0; i < appointments->size(); ++i) {
        const Appointment& a = appointments->getAt(i);
        if (a.getDoctorId() == docId && a.getDate() == date &&
            a.getTimeSlot() == slot && a.getStatus() != "cancelled")
            return true;
    }
    return false;
}

// Auto-mark any past pending appointments as "missed" and save to file.
void PatientScreen::autoMarkMissed() {
    time_t t = time(nullptr);
    tm now{};
    localtime_s(&now, &t);
    int todayInt = (now.tm_year + 1900) * 10000 + (now.tm_mon + 1) * 100 + now.tm_mday;
    int curHour = now.tm_hour;
    std::string today = getTodayDate();

    bool changed = false;
    for (int i = 0; i < appointments->size(); ++i) {
        const Appointment& a = appointments->getAt(i);
        if (a.getStatus() != "pending") continue;

        int apptInt = dateToInt(a.getDate());
        bool missed = false;

        if (apptInt < todayInt) {
            missed = true;  
        }
        else if (a.getDate() == today) {
            int slotHour = std::stoi(a.getTimeSlot().substr(0, 2));
            if (slotHour < curHour) missed = true;  
        }

        if (missed) {
            Appointment* ap = appointments->findById(a.getAppointmentId());
            if (ap) { ap->setStatus("missed"); changed = true; }
        }
    }
    if (changed) FileHandler::saveAllAppointments(*appointments);
}

//View setup helpers

void PatientScreen::setupViewAppts() {
    // Auto-update any past pending appointments to "missed" before displaying
    autoMarkMissed();

    viewTitle.setString("My Appointments");
    listRows.clear(); scrollOffset = 0;

    int idx[200]; int n = 0;
    for (int i = 0; i < appointments->size(); ++i)
        if (appointments->getAt(i).getPatientId() == patient->getId())
            idx[n++] = i;

    // Sort ascending by date
    for (int i = 1; i < n; ++i) {
        int key = idx[i], j = i - 1;
        while (j >= 0 && dateToInt(appointments->getAt(idx[j]).getDate()) >
            dateToInt(appointments->getAt(key).getDate()))
        {
            idx[j + 1] = idx[j]; --j;
        }
        idx[j + 1] = key;
    }

    listRows.push_back("ID|Doctor|Specialization|Date|Slot|Status");
    listRows.push_back(std::string(82, '-'));

    for (int i = 0; i < n; ++i) {
        const Appointment& a = appointments->getAt(idx[i]);
        Doctor* d = doctors->findById(a.getDoctorId());
        std::string row =
            std::to_string(a.getAppointmentId()) + "|" +
            (d ? d->getName() : "Unknown") + "|" +
            (d ? d->getSpecialization() : "-") + "|" +
            a.getDate() + "|" +
            a.getTimeSlot() + "|" +
            a.getStatus();
        listRows.push_back(row);
    }
    if (n == 0) listRows.push_back("No appointments found.");
}

void PatientScreen::setupViewRecords() {
    viewTitle.setString("My Medical Records");
    listRows.clear(); scrollOffset = 0;

    int idx[200]; int n = 0;
    for (int i = 0; i < prescriptions->size(); ++i)
        if (prescriptions->getAt(i).getPatientId() == patient->getId())
            idx[n++] = i;
    for (int i = 1; i < n; ++i) {
        int key = idx[i], j = i - 1;
        while (j >= 0 && dateToInt(prescriptions->getAt(idx[j]).getDate()) <
            dateToInt(prescriptions->getAt(key).getDate()))
        {
            idx[j + 1] = idx[j]; --j;
        }
        idx[j + 1] = key;
    }
    for (int i = 0; i < n; ++i) {
        const Prescription& p = prescriptions->getAt(idx[i]);
        Doctor* d = doctors->findById(p.getDoctorId());
        listRows.push_back("Date:      " + p.getDate());
        listRows.push_back("Doctor:    Dr. " + d->getName());
        listRows.push_back("  Medicines: " + p.getMedicines());
        listRows.push_back("  Notes:     " + p.getNotes());
        listRows.push_back("");
    }
    if (n == 0) listRows.push_back("No medical records found.");
}

void PatientScreen::setupViewBills() {
    viewTitle.setString("My Bills");
    listRows.clear(); scrollOffset = 0;
    float total = 0.f;

    listRows.push_back("BillID|ApptID|Amount (PKR)|Status|Date");
    listRows.push_back(std::string(60, '-'));

    bool any = false;
    for (int i = 0; i < bills->size(); ++i) {
        const Bill& b = bills->getAt(i);
        if (b.getPatientId() != patient->getId()) continue;
        std::ostringstream amt;
        amt << std::fixed << std::setprecision(2) << b.getAmount();
        std::string row =
            std::to_string(b.getBillId()) + "|" +
            std::to_string(b.getAppointmentId()) + "|" +
            "PKR " + amt.str() + "|" +
            b.getStatus() + "|" +
            b.getDate();
        listRows.push_back(row);
        if (b.getStatus() == "unpaid") total += b.getAmount();
        any = true;
    }
    if (!any) { listRows.push_back("No bills found."); return; }
    listRows.push_back("");
    std::ostringstream ts;
    ts << "Total unpaid: PKR " << std::fixed << std::setprecision(2) << total;
    listRows.push_back(ts.str());
}

void PatientScreen::setupBook() {
    viewTitle.setString("Book Appointment");
    bookStep = BookStep::SPEC_SEARCH;
    message.clear(); listRows.clear(); scrollOffset = 0;
    foundDoctorIds.clear(); availableSlots.clear(); slotButtons.clear();
    selectedDoctorId = -1; bookDate.clear();

    lInput1.setString("Specialization");
    lInput1.setPosition(240.f, 500.f);
    tbInput1 = TextBox(font, "e.g. Cardiology", { 240.f, 522.f }, { 400.f, Theme::INPUT_H });
    btnConfirm.setText("Search Doctors", font);
    btnConfirm.setPosition({ 240.f, 580.f });
    btnBack2.setPosition({ 450.f, 580.f });
}

void PatientScreen::setupCancel() {
    viewTitle.setString("Cancel Appointment");
    listRows.clear(); scrollOffset = 0;
    message.clear();

    listRows.push_back("ID|Doctor|Date|Slot|Status");
    listRows.push_back(std::string(60, '-'));
    bool any = false;
    for (int i = 0; i < appointments->size(); ++i) {
        const Appointment& a = appointments->getAt(i);
        if (a.getPatientId() != patient->getId() || a.getStatus() != "pending") continue;
        Doctor* d = doctors->findById(a.getDoctorId());
        std::string row =
            std::to_string(a.getAppointmentId()) + "|" +
            (d ? d->getName() : "Unknown") + "|" +
            a.getDate() + "|" +
            a.getTimeSlot() + "|" +
            a.getStatus();
        listRows.push_back(row);
        any = true;
    }
    if (!any) listRows.push_back("No pending appointments.");

    lInput1.setString("Appointment ID to cancel");
    lInput1.setPosition(240.f, 500.f);
    tbInput1 = TextBox(font, "Enter Appointment ID", { 240.f, 522.f }, { 300.f, Theme::INPUT_H });
    btnConfirm.setText("Cancel Appointment", font);
    btnConfirm.setPosition({ 240.f, 580.f });
    btnBack2.setPosition({ 450.f, 580.f });
}

void PatientScreen::setupPayBill() {
    viewTitle.setString("Pay Bill");
    listRows.clear(); scrollOffset = 0;
    message.clear();

    listRows.push_back("BillID|ApptID|Amount (PKR)|Date");
    listRows.push_back(std::string(50, '-'));
    bool any = false;
    for (int i = 0; i < bills->size(); ++i) {
        const Bill& b = bills->getAt(i);
        if (b.getPatientId() != patient->getId() || b.getStatus() != "unpaid") continue;
        std::ostringstream amt;
        amt << std::fixed << std::setprecision(2) << b.getAmount();
        std::string row =
            std::to_string(b.getBillId()) + "|" +
            std::to_string(b.getAppointmentId()) + "|" +
            "PKR " + amt.str() + "|" +
            b.getDate();
        listRows.push_back(row);
        any = true;
    }
    if (!any) listRows.push_back("No unpaid bills.");

    lInput1.setString("Bill ID to pay");
    lInput1.setPosition(240.f, 500.f);
    tbInput1 = TextBox(font, "Enter Bill ID", { 240.f, 522.f }, { 300.f, Theme::INPUT_H });
    btnConfirm.setText("Pay Bill", font);
    btnConfirm.setPosition({ 240.f, 580.f });
    btnBack2.setPosition({ 450.f, 580.f });
}

void PatientScreen::setupTopUp() {
    viewTitle.setString("Top Up Balance");
    message.clear(); listRows.clear();
    lInput1.setString("Amount to add (PKR)");
    lInput1.setPosition(240.f, 80.f);
    tbInput1 = TextBox(font, "e.g. 1000", { 240.f, 102.f }, { 300.f, Theme::INPUT_H });
    btnConfirm.setText("Add Balance", font);
    btnConfirm.setPosition({ 240.f, 162.f });
    btnBack2.setPosition({ 450.f, 162.f });
}

void PatientScreen::doBookStep1() {
    std::string spec = tbInput1.getValue();
    if (spec.empty()) { message = "Enter a specialization."; msgSuccess = false; return; }

    foundDoctorIds.clear();
    listRows.clear(); listRows.push_back("ID    Name                Fee");
    listRows.push_back(std::string(40, '-'));
    for (int i = 0; i < doctors->size(); ++i) {
        const Doctor& d = doctors->getAt(i);
        if (strEqualCI(d.getSpecialization(), spec)) {
            std::ostringstream ss;
            ss << std::left << std::setw(6) << d.getId()
                << std::setw(20) << d.getName()
                << "PKR " << std::fixed << std::setprecision(2) << d.getFee();
            listRows.push_back(ss.str());
            foundDoctorIds.push_back(d.getId());
        }
    }
    if (foundDoctorIds.empty()) {
        listRows.push_back("No doctors found for: " + spec);
        message = "No doctors available for that specialization.";
        msgSuccess = false; return;
    }

    bookStep = BookStep::SLOT_SELECT;
    lInput1.setString("Doctor ID"); lInput1.setPosition(240.f, 320.f);
    tbInput1 = TextBox(font, "Enter Doctor ID", { 240.f, 342.f }, { 300.f, Theme::INPUT_H });
    lInput2.setString("Date (DD-MM-YYYY)"); lInput2.setPosition(240.f, 400.f);
    tbInput2 = TextBox(font, "e.g. 31-12-2026", { 240.f, 422.f }, { 300.f, Theme::INPUT_H });
    btnConfirm.setText("Show Slots", font);
    btnConfirm.setPosition({ 240.f, 480.f });
    btnBack2.setPosition({ 450.f, 480.f });
    message.clear();
}

void PatientScreen::doBookStep2() {
    std::string docIdStr = tbInput1.getValue();
    std::string date = tbInput2.getValue();

    int docId = 0;
    try { docId = std::stoi(docIdStr); }
    catch (...) { message = "Invalid Doctor ID."; msgSuccess = false; return; }

    bool fmtOk = (date.size() == 10 && date[2] == '-' && date[5] == '-');
    if (fmtOk) {
        for (int pos : {0, 1, 3, 4, 6, 7, 8, 9})
            if (date[pos] < '0' || date[pos] > '9') { fmtOk = false; break; }
    }
    if (!fmtOk) {
        message = "Invalid format. Use DD-MM-YYYY (e.g. 25-12-2026).";
        msgSuccess = false; return;
    }

    // Step 2: check day and month are in valid range
    int day = 0, month = 0, year = 0;
    try {
        day = std::stoi(date.substr(0, 2));
        month = std::stoi(date.substr(3, 2));
        year = std::stoi(date.substr(6, 4));
    }
    catch (...) { message = "Invalid date values."; msgSuccess = false; return; }

    if (day < 1 || day > 31 || month < 1 || month > 12) {
        message = "Invalid day or month in date."; msgSuccess = false; return;
    }

    // Step 3: reject past dates with a clear, specific message
    if (!Validator::isValidDate(date)) {
        message = "That date has already passed. Please choose today or a future date.";
        msgSuccess = false; return;
    }

    // Check doctor is in found list
    bool valid = false;
    for (int id : foundDoctorIds) if (id == docId) { valid = true; break; }
    if (!valid) { message = "Doctor not found in results."; msgSuccess = false; return; }

    selectedDoctorId = docId;
    bookDate = date;

    // Filter past time slots when the selected date is today
    std::string today = getTodayDate();
    bool isToday = (date == today);
    int curHour = 0;
    if (isToday) {
        time_t t = time(nullptr);
        tm now{};
        localtime_s(&now, &t);
        curHour = now.tm_hour;
    }

    availableSlots.clear(); slotButtons.clear();
    float sx = 240.f, sy = 560.f;
    for (int i = 0; i < SLOT_COUNT; ++i) {
        // Skip slots whose hour has already passed for today
        if (isToday) {
            int slotHour = std::stoi(SLOTS[i].substr(0, 2));
            if (slotHour < curHour) continue;
        }
        if (!isSlotTaken(docId, date, SLOTS[i])) {
            availableSlots.push_back(SLOTS[i]);
            Button b(font, SLOTS[i], { sx, sy }, { 90.f, 36.f }, Button::Style::GHOST);
            slotButtons.push_back(b);
            sx += 100.f;
            if (sx > 1100.f) { sx = 240.f; sy += 46.f; }
        }
    }
    if (availableSlots.empty()) {
        if (isToday)
            message = "All time slots for today have already passed. Please choose a future date.";
        else
            message = "No slots available on " + date + " — all slots are already booked.";
        msgSuccess = false; return;
    }

    bookStep = BookStep::DONE;
    message = "Select a time slot below:"; msgSuccess = true;
    listRows.push_back("");
    listRows.push_back("Available slots on " + date + ":");
    for (auto& s : availableSlots) listRows.push_back("  " + s);
}

void PatientScreen::doConfirmBook() {
    if (bookStep == BookStep::SPEC_SEARCH) { doBookStep1(); return; }
    if (bookStep == BookStep::SLOT_SELECT) { doBookStep2(); return; }
}

void PatientScreen::doConfirmCancel() {
    std::string inp = tbInput1.getValue();
    int apptId = 0;
    try { apptId = std::stoi(inp); }
    catch (...) { message = "Invalid ID."; msgSuccess = false; return; }

    Appointment* a = appointments->findById(apptId);
    if (!a || a->getPatientId() != patient->getId() || a->getStatus() != "pending") {
        message = "Invalid appointment ID."; msgSuccess = false; return;
    }
    Doctor* d = doctors->findById(a->getDoctorId());
    float fee = d ? d->getFee() : 0.f;
    a->setStatus("cancelled");
    FileHandler::saveAllAppointments(*appointments);
    *patient += fee;
    FileHandler::saveAllPatients(*patients);
    for (int i = 0; i < bills->size(); ++i)
        if (bills->getAt(i).getAppointmentId() == apptId) {
            bills->getAt(i).setStatus("cancelled");
            FileHandler::saveAllBills(*bills); break;
        }
    std::ostringstream ss;
    ss << "Cancelled. PKR " << std::fixed << std::setprecision(2) << fee << " refunded.";
    message = ss.str(); msgSuccess = true;
    refreshHeader(); setupCancel();
}

void PatientScreen::doConfirmPayBill() {
    std::string inp = tbInput1.getValue();
    int billId = 0;
    try { billId = std::stoi(inp); }
    catch (...) { message = "Invalid ID."; msgSuccess = false; return; }

    Bill* b = bills->findById(billId);
    if (!b || b->getPatientId() != patient->getId() || b->getStatus() != "unpaid") {
        message = "Invalid Bill ID."; msgSuccess = false; return;
    }
    if (patient->getBalance() < b->getAmount()) {
        message = "Insufficient balance."; msgSuccess = false; return;
    }
    *patient -= b->getAmount();
    b->setStatus("paid");
    FileHandler::saveAllBills(*bills);
    FileHandler::saveAllPatients(*patients);
    std::ostringstream ss;
    ss << "Paid! Remaining balance: PKR " << std::fixed << std::setprecision(2) << patient->getBalance();
    message = ss.str(); msgSuccess = true;
    refreshHeader(); setupPayBill();
}

void PatientScreen::doConfirmTopUp() {
    std::string inp = tbInput1.getValue();
    if (!Validator::isValidPositiveFloat(inp)) {
        message = "Enter a positive amount."; msgSuccess = false; return;
    }
    float amt = std::stof(inp);
    *patient += amt;
    FileHandler::saveAllPatients(*patients);
    std::ostringstream ss;
    ss << "Balance updated! New balance: PKR " << std::fixed << std::setprecision(2) << patient->getBalance();
    message = ss.str(); msgSuccess = true;
    refreshHeader(); tbInput1.clear();
}

//Event Handling

void PatientScreen::handleEvent(const sf::Event& e, sf::RenderWindow& win) {
    if (btnBook.handleEvent(e, win)) { currentView = View::BOOK;        setupBook();        return; }
    if (btnCancel.handleEvent(e, win)) { currentView = View::CANCEL;       setupCancel();      return; }
    if (btnViewAppts.handleEvent(e, win)) { currentView = View::VIEW_APPTS;   setupViewAppts();   return; }
    if (btnRecords.handleEvent(e, win)) { currentView = View::VIEW_RECORDS; setupViewRecords(); return; }
    if (btnBills.handleEvent(e, win)) { currentView = View::VIEW_BILLS;   setupViewBills();   return; }
    if (btnPayBill.handleEvent(e, win)) { currentView = View::PAY_BILL;     setupPayBill();     return; }
    if (btnTopUp.handleEvent(e, win)) { currentView = View::TOPUP;        setupTopUp();       return; }
    if (btnLogout.handleEvent(e, win)) { nextScreen = ScreenID::MAIN_MENU; return; }

    if (e.type == sf::Event::MouseWheelScrolled) {
        scrollOffset -= (int)e.mouseWheelScroll.delta * 2;
        if (scrollOffset < 0) scrollOffset = 0;
    }

    if (currentView == View::BOOK) {
        if (bookStep != BookStep::DONE) {
            tbInput1.handleEvent(e, win); tbInput2.handleEvent(e, win);
        }
        if (btnConfirm.handleEvent(e, win)) doConfirmBook();
        if (btnBack2.handleEvent(e, win)) { currentView = View::MENU; message.clear(); }

        for (int i = 0; i < (int)slotButtons.size(); ++i) {
            if (slotButtons[i].handleEvent(e, win)) {
                std::string slot = availableSlots[i];
                Doctor* d = doctors->findById(selectedDoctorId);
                if (!d) { message = "Doctor not found."; msgSuccess = false; return; }
                if (patient->getBalance() < d->getFee()) {
                    message = "Insufficient balance."; msgSuccess = false; return;
                }
                int newApptId = 1;
                for (int j = 0; j < appointments->size(); ++j)
                    if (appointments->getAt(j).getAppointmentId() >= newApptId)
                        newApptId = appointments->getAt(j).getAppointmentId() + 1;
                std::string today = getTodayDate();
                Appointment appt(newApptId, patient->getId(), selectedDoctorId,
                    bookDate, slot, "pending");
                appointments->add(appt);
                FileHandler::appendAppointment(appt);
                int newBillId = 1;
                for (int j = 0; j < bills->size(); ++j)
                    if (bills->getAt(j).getBillId() >= newBillId)
                        newBillId = bills->getAt(j).getBillId() + 1;
                Bill bill(newBillId, patient->getId(), newApptId,
                    d->getFee(), "unpaid", today);
                bills->add(bill); FileHandler::appendBill(bill);
                std::ostringstream ss;
                ss << "Booked! Appt ID: " << newApptId
                    << " | PKR " << std::fixed << std::setprecision(2)
                    << d->getFee() << " deducted.";
                message = ss.str(); msgSuccess = true;
                refreshHeader();
                currentView = View::VIEW_APPTS;
                setupViewAppts();
                return;
            }
        }
    }
    else if (currentView == View::CANCEL) {
        tbInput1.handleEvent(e, win);
        if (btnConfirm.handleEvent(e, win)) doConfirmCancel();
        if (btnBack2.handleEvent(e, win)) { currentView = View::MENU; message.clear(); }
    }
    else if (currentView == View::PAY_BILL) {
        tbInput1.handleEvent(e, win);
        if (btnConfirm.handleEvent(e, win)) doConfirmPayBill();
        if (btnBack2.handleEvent(e, win)) { currentView = View::MENU; message.clear(); }
    }
    else if (currentView == View::TOPUP) {
        tbInput1.handleEvent(e, win);
        if (btnConfirm.handleEvent(e, win)) doConfirmTopUp();
        if (btnBack2.handleEvent(e, win)) { currentView = View::MENU; message.clear(); }
    }
}

void PatientScreen::update(float dt) {
    tbInput1.update(dt); tbInput2.update(dt); tbInput3.update(dt);
    msgText.setString(message);
    msgText.setFillColor(msgSuccess ? Theme::SUCCESS : Theme::DANGER);
}

//Drawing

void PatientScreen::drawList(sf::RenderWindow& win)
{
    win.draw(listBg);
    float y = 78.f;
    float lineH = 24.f;
    int start = scrollOffset;
    int maxLines = (int)((Theme::WIN_H - 120.f) / lineH);

    static const float COL_X[] = { 238.f, 298.f, 378.f, 518.f, 638.f, 700.f, 780.f };

    sf::Text cell;
    cell.setFont(font);
    cell.setCharacterSize(14);

    for (int i = start; i < (int)listRows.size() && i < start + maxLines; ++i)
    {
        const std::string& row = listRows[i];

        if (i > 1 && !row.empty() && row[0] != '-')
        {
            sf::RectangleShape bg({ (float)Theme::WIN_W - 240.f, lineH - 2.f });
            bg.setPosition(230.f, y);
            bg.setFillColor((i % 2 == 0) ? Theme::ROW_EVEN : Theme::ROW_ODD);
            win.draw(bg);
        }

        if (row.find('|') != std::string::npos)
        {
            std::istringstream ss(row);
            std::string col;
            int c = 0;
            while (std::getline(ss, col, '|') && c < 7)
            {
                cell.setString(col);
                cell.setPosition(COL_X[c], y);
                cell.setFillColor(i < 2 ? Theme::ACCENT : Theme::TEXT_WHITE);
                win.draw(cell);
                ++c;
            }
        }
        else
        {
            cell.setString(row);
            cell.setPosition(238.f, y);
            cell.setFillColor(row.substr(0, 5) == "Date:" ? Theme::ACCENT : Theme::TEXT_WHITE);
            win.draw(cell);
        }

        y += lineH;
    }
}

void PatientScreen::draw(sf::RenderWindow& win) {
    win.draw(contentArea);
    win.draw(sidebar);
    win.draw(topBar);
    win.draw(headerName);
    win.draw(headerBalance);

    btnBook.draw(win); btnCancel.draw(win); btnViewAppts.draw(win);
    btnRecords.draw(win); btnBills.draw(win); btnPayBill.draw(win);
    btnTopUp.draw(win); btnLogout.draw(win);

    win.draw(viewTitle);

    if (currentView == View::VIEW_APPTS || currentView == View::VIEW_RECORDS ||
        currentView == View::VIEW_BILLS || currentView == View::CANCEL ||
        currentView == View::PAY_BILL) {
        drawList(win);
    }

    if (currentView == View::BOOK || currentView == View::CANCEL ||
        currentView == View::PAY_BILL || currentView == View::TOPUP) {
        if (currentView == View::BOOK) {
            drawList(win);
            for (auto& sb : slotButtons) sb.draw(win);
        }
        if (!(currentView == View::BOOK && bookStep == BookStep::DONE)) {
            win.draw(lInput1); tbInput1.draw(win);
            if (currentView == View::BOOK && bookStep == BookStep::SLOT_SELECT) {
                win.draw(lInput2); tbInput2.draw(win);
            }
            btnConfirm.draw(win);
        }
        btnBack2.draw(win);
    }

    if (currentView == View::TOPUP) { win.draw(lInput1); tbInput1.draw(win); }

    if (currentView == View::BOOK) {
        if (bookStep == BookStep::SLOT_SELECT)
            msgText.setPosition(240.f, 540.f);   
        else if (bookStep == BookStep::SPEC_SEARCH)
            msgText.setPosition(240.f, 640.f);   
        else
            msgText.setPosition(240.f, 640.f);
    }
    else {
        msgText.setPosition(240.f, 640.f);       
    }
    win.draw(msgText);
}
