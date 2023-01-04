//
// Created by zhangx on 2022/3/18.
//

#include "Material.h"


Material::~Material() {

}

double Material::getE() const {
    return E;
}

double Material::getMi() const {
    return mi;
}

double Material::getG() const {
    return g;
}

double Material::getRo() const {
    return ro;
}

void Material::setE(double e) {
    E = e;
}

void Material::setMi(double mi) {
    Material::mi = mi;
}

void Material::setG(double g) {
    Material::g = g;
}

void Material::setRo(double ro) {
    Material::ro = ro;
}

double Material::getT() const {
    return t;
}

void Material::setT(double t) {
    Material::t = t;
}

Material::Material(double E, double mi, double ro, double g, double t):E(E),mi(mi),ro(ro),g(g),t(t) {
}
