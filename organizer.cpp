#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <filesystem>
#include <unordered_map>
#include <vector>
#include <string>
#include <cstdlib> // Para getenv

namespace fs = std::filesystem;

void organizarDescargas() {
    std::string carpetaDescargas = (fs::path(getenv("USERPROFILE")) / "Downloads").string();

    // Mapa con subcarpetas y extensiones asociadas
    std::unordered_map<std::string, std::vector<std::string>> subcarpetas = {
        {"Documentos", {".pdf", ".docx", ".xlsx", ".pptx", ".txt"}},
        {"Imagenes", {".jpg", ".jpeg", ".png", ".gif"}},
        {"Videos", {".mp4", ".mov", ".avi"}},
        {"Programas", {".exe", ".msi"}},
        {"Comprimidos", {".zip", ".rar", ".7z"}},
        {"Otros", {}}
    };

    for (const auto& entrada : fs::directory_iterator(carpetaDescargas)) {
        if (fs::is_regular_file(entrada)) {
            std::string archivo = entrada.path().filename().string();
            std::string extension = entrada.path().extension().string();
            bool movido = false;

            for (const auto& [subcarpeta, extensiones] : subcarpetas) {
                if (std::find(extensiones.begin(), extensiones.end(), extension) != extensiones.end()) {
                    std::string carpetaDestino = carpetaDescargas + "/" + subcarpeta;
                    fs::create_directories(carpetaDestino);
                    fs::rename(entrada.path(), fs::path(carpetaDestino) / archivo);
                    movido = true;
                    break;
                }
            }

            if (!movido) {
                std::string carpetaDestino = carpetaDescargas + "/Otros";
                fs::create_directories(carpetaDestino);
                fs::rename(entrada.path(), fs::path(carpetaDestino) / archivo);
            }
        }
    }
}

int main() {
    organizarDescargas();
    return 0;
}
