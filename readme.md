# 🖼️ Image Processor – PPR Projekt Edition

Ein Projekt zur parallelen Bildverarbeitung in C mit Weboberfläche in Python (Flask).  
Du kannst Bilder hochladen, eine Toleranz für Hintergrundfarbe wählen, und Filter anwenden (z. B. Graustufen).

---

## 📁 Projektstruktur

```
PPR-PROJEKT/
│
├── image_processor/           # C-Code + Executable
│   ├── main.c                 # C-Programm
│   ├── libs/                  # stb_image / stb_image_write
│   └── assets/                # Beispielbilder/ Bilder fuers Testen
│
├── webapp/                    # Python Flask Web-App
│   ├── app.py                 # Backend-Logik
│   ├── templates/index.html   # HTML-Frontend
│   └── static/                # Output-Bilder (werden automatisch erstellt)
│
├── .gitignore
└── README.md
```

---

## ⚙️ C-Programm kompilieren

### 📦 Voraussetzungen
- GCC mit OpenMP-Unterstützung
- Windows: z. B. `MinGW64`  
- Linux/macOS: Standard-GCC funktioniert

### 🔨 Kompilieren

```bash
gcc main.c -o lammbock.exe -fopenmp -lm
```

> Achtung: Das Programm erwartet `stb_image.h` und `stb_image_write.h` im Unterordner `libs/`.

---

## ▶️ C-Programm ausführen

```bash
./lammbock.exe <input.jpg> <output.png> <toleranz>
```

Beispiel:

```bash
./lammbock.exe assets/img_1.jpg webapp/static/output.png 30
```

---

## 🌐 Web-Oberfläche starten

### 📦 Voraussetzungen
- Python 3.x
- Flask (`pip install flask`)

### 💡 Optional: Virtuelle Umgebung

```bash
python -m venv venv
source venv/bin/activate   # Linux/macOS
venv\Scripts\activate      # Windows
pip install flask
```

### 🚀 Start

```bash
cd webapp
python app.py
```

### 🌍 Anwendung aufrufen

Öffne deinen Browser und gehe zu:

```
http://127.0.0.1:5000/
```

---

## 🧼 Hinweis

- Hochgeladene Bilder werden in `webapp/static/` gespeichert
- Standardmäßig heißt das Bild `input.jpg`, Ausgabe ist `output.png`
- Im Frontend kannst du:
  - Bild hochladen
  - Toleranz (Farbschwelle) wählen
  - Filter anwenden starten
  - Vorher-Nachher angucken

---

## ✨ Weiterführend

- TODOs: Automatische Dateibenennung mit UUIDs? sonst wird das letzte Bild überschrieben
- Erweiterung um mehr Filter? aktuell geht nur Maskenerkennung + SW Filter.. kann man bestimmt noch erweitern
- Aktuell wird über Flask noch keine Threadzahl mitgegeben, könnte man noch checken

