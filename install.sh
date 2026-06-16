#!/bin/sh
set -e

REPO="https://github.com/zetatez/hhkb.git"
PREFIX="${PREFIX:-/usr/local}"
BINDIR="${PREFIX}/bin"
MANDIR="${PREFIX}/share/man"
SERVICEDIR="${HOME}/.config/systemd/user"

err() { echo "[!] $*" >&2; exit 1; }
info() { echo "[*] $*"; }
done_() { echo "[+] $*"; }

cleanup() {
  [ -n "${tmpdir}" ] && rm -rf "${tmpdir}"
}
trap cleanup EXIT

# ---- check dependencies ----
command -v cc  >/dev/null 2>&1 || err "cc (C compiler) not found"
command -v make >/dev/null 2>&1 || err "make not found"
command -v pkg-config >/dev/null 2>&1 || err "pkg-config not found"

info "Checking X11 dependencies..."
for pkg in x11 xi xfixes xext xinerama xtst xrandr xft fontconfig; do
  pkg-config --exists "$pkg" 2>/dev/null || err "missing X11 library: $pkg"
done

# ---- download ----
tmpdir="$(mktemp -d)"
info "Downloading hhkb from ${REPO}"

if command -v git >/dev/null 2>&1; then
  git clone --depth=1 "${REPO}" "${tmpdir}/hhkb"
else
  command -v curl >/dev/null 2>&1 || err "need git or curl"
  curl -fsSL "${REPO%.git}/archive/refs/heads/master.tar.gz" | tar xz -C "${tmpdir}"
  mv "${tmpdir}"/hhkb-* "${tmpdir}/hhkb"
fi

cd "${tmpdir}/hhkb"

# ---- build ----
info "Building hhkb..."
make

# ---- install binary & man page ----
info "Installing to ${PREFIX}..."
gzip -c hhkb.1 > hhkb.1.gz
[ -d "${BINDIR}" ] || mkdir -p "${BINDIR}" 2>/dev/null || sudo mkdir -p "${BINDIR}"
[ -d "${MANDIR}/man1" ] || mkdir -p "${MANDIR}/man1" 2>/dev/null || sudo mkdir -p "${MANDIR}/man1"
install -m755 hhkb "${BINDIR}/" 2>/dev/null || sudo install -m755 hhkb "${BINDIR}/"
install -m644 hhkb.1.gz "${MANDIR}/man1/" 2>/dev/null || sudo install -m644 hhkb.1.gz "${MANDIR}/man1/"
done_ "Installed hhkb to ${BINDIR}/hhkb"
done_ "Installed man page to ${MANDIR}/man1/hhkb.1.gz"

# ---- install systemd user service ----
mkdir -p "${SERVICEDIR}"
install -m644 hhkb.service "${SERVICEDIR}/"
systemctl --user disable --now hhkb 2>/dev/null || true
systemctl --user daemon-reload
systemctl --user enable --now hhkb
done_ "Installed and started systemd user service"

# ---- PATH hint ----
case ":${PATH}:" in
  *:"${BINDIR}":*) ;;
  *) echo "[~] ${BINDIR} is not in PATH. Add 'export PATH=\"${BINDIR}:\$PATH\"' to your ~/.profile or ~/.bashrc" ;;
esac

info "Installation complete. hhkb is now running."
