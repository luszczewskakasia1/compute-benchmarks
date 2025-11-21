# CUDA Benchmarks Setup Guide

This guide provides detailed instructions for installing the NVIDIA GPU drivers and CUDA Toolkit required for running CUDA benchmarks on both Windows and Linux systems.

## Table of Contents

- [Prerequisites](#prerequisites)
- [Installation Steps](#installation-steps)
  - [Step 1: Install NVIDIA GPU Drivers](#step-1-install-nvidia-gpu-drivers)
  - [Step 2: Install CUDA Toolkit](#step-2-install-cuda-toolkit)
    - [Windows Installation](#windows-installation)
    - [Linux Installation](#linux-installation)
- [Post-Installation Verification](#post-installation-verification)
- [Building CUDA Benchmarks](#building-cuda-benchmarks)
- [Troubleshooting](#troubleshooting)

## Prerequisites

#### Common Prerequisites

**NVIDIA GPU**: A CUDA-capable NVIDIA GPU
- Check compatibility at: https://developer.nvidia.com/cuda-gpus

#### Windows-Specific Prerequisites

**IMPORTANT**: Visual Studio **should** be installed **before** the CUDA Toolkit
If Visual Studio is installed after the CUDA Toolkit, integration files may not be found automatically and will require manual configuration

#### Linux-Specific Prerequisites

- **GCC Compiler**: Compatible version (check CUDA Toolkit release notes for supported versions)
- **Build Tools**: `make`, `gcc`, `g++`

## Installation Steps

### Step 1: Install NVIDIA GPU Drivers

**Note**: NVIDIA GPU drivers should be installed before the CUDA Toolkit.

#### Windows Driver Installation

1. Visit the NVIDIA Driver Downloads page: https://www.nvidia.com/Download/index.aspx
2. Select your GPU model, OS version, and download type
3. Follow instructions provided on the website
4. Restart your system when installation completes

**Verify installation:**
```cmd
nvidia-smi
```
Expected output: GPU information and driver version

On Linux you can install NVIDIA drivers using your package manager or by downloading directly from NVIDIA.

```bash
# Ubuntu/Debian
sudo apt update
sudo apt install nvidia-driver-XXX
```

Replace `XXX` with the recommended driver version for your GPU (e.g., `535`, `545`).

To find available driver versions:
```bash
apt search nvidia-driver
```

**Note**: If `nvidia-smi` executes but shows no CUDA-capable devices, or if you encounter driver-related issues, try reinstalling the driver.

---

### Step 2: Install CUDA Toolkit

#### Windows Installation

**a) Install Visual Studio (if not already installed)**

**Important:** Supported versions for CUDA Toolkit are listed in: https://docs.nvidia.com/cuda/cuda-installation-guide-microsoft-windows/index.html#system-requirements
If you try to install the CUDA Toolkit with unsupported Visual Studio versions, the installer will warn you.

1. Download Visual Studio from: https://visualstudio.microsoft.com/downloads/
2. Run the installer and select "Desktop development with C++"
3. Complete the installation and restart your system if prompted

**Note**: This step must be completed before installing the CUDA Toolkit to ensure proper integration.

**b) Download CUDA Toolkit**

1. Visit the NVIDIA CUDA Downloads page: https://developer.nvidia.com/cuda-downloads
2. Select your operating system:
   - Operating System: Windows
   - Architecture: x86_64
   - Version: Your Windows version
   - Installer Type: exe (network) or exe (local)
3. Download the installer

**c) Install CUDA Toolkit**

1. Run the downloaded installer (e.g., `cuda_XX.X.X_windows.exe`)
2. Choose installation options:
   - **Express Installation** (Recommended): Installs driver, toolkit, samples, and documentation
3. Follow the installation wizard prompts
4. Wait for installation to complete (this may take several minutes)

After installation environment variables are usually set automatically (e.g., `CUDA_PATH`, `CUDA_PATH_V13_0`). If not, refer to substep (e).

**d) Configure Visual Studio Integration (If Needed)**

If Visual Studio was installed after the CUDA Toolkit, or if the integration is not working:

The recommended approach is to reinstall the CUDA Toolkit. During installation, select the **Custom/Advanced** option and ensure that **Visual Studio Integration** is checked.

**e) Set Environment Variables**

The installer typically sets these automatically. Verify they exist:

- `CUDA_PATH` & `CUDA_PATH_VXX_X`: `C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\vXX.X`

---

#### Linux Installation

##### Option 1: Quick Installation via Package Manager (Recommended)

The easiest way to install the CUDA Toolkit is using your system's package manager. This method installs the latest available version with minimal configuration required.

**For Ubuntu/Debian:**
```bash
sudo apt update
sudo apt install nvidia-cuda-toolkit
```

The package manager automatically:
- Installs the CUDA Toolkit and all dependencies
- Sets up environment variables
- Configures system paths

You can now proceed to [Post-Installation Verification](#post-installation-verification).

**Note**: This installs the version available in your distribution's repositories, which may not be the latest CUDA release. If you need a specific CUDA version, use Option 2 below.

---

##### Option 2: Manual Installation for Specific CUDA Version

If you require a specific CUDA Toolkit version (e.g., for compatibility requirements), follow these steps to download and install directly from NVIDIA.

**Download CUDA Toolkit**

1. Visit the NVIDIA CUDA Downloads page: https://developer.nvidia.com/cuda-downloads
2. Select your configuration:
   - **Operating System**: Linux
   - **Architecture**: x86_64
   - **Distribution**: Your Linux distribution (e.g., Ubuntu)
   - **Version**: Your distribution version
   - **Installer Type**: Choose based on preference (deb/rpm recommended)

**Install CUDA Toolkit**

**Example for Ubuntu 24.04 (CUDA 13.0):**

```bash
# Download the repository pin file
wget https://developer.download.nvidia.com/compute/cuda/repos/ubuntu2404/x86_64/cuda-ubuntu2404.pin
sudo mv cuda-ubuntu2404.pin /etc/apt/preferences.d/cuda-repository-pin-600

# Download and install the repository package
wget https://developer.download.nvidia.com/compute/cuda/13.0.2/local_installers/cuda-repo-ubuntu2404-13-0-local_13.0.2-580.95.05-1_amd64.deb
sudo dpkg -i cuda-repo-ubuntu2404-13-0-local_13.0.2-580.95.05-1_amd64.deb

# Add the CUDA repository key
sudo cp /var/cuda-repo-ubuntu2404-13-0-local/cuda-*-keyring.gpg /usr/share/keyrings/

# Update package list and install specific CUDA version
sudo apt-get update
sudo apt-get -y install cuda-toolkit-13-0
```

**Set Environment Variables**

After manual installation, configure your environment:

**For bash** (edit `~/.bashrc`):
```bash
export CUDA_HOME=/usr/local/cuda-13.0
export PATH=$CUDA_HOME/bin:$PATH
export LD_LIBRARY_PATH=$CUDA_HOME/lib64:$LD_LIBRARY_PATH
```

Apply the changes:
```bash
source ~/.bashrc
```

**Note**: Replace `13.0` with your installed version number. The installation creates `/usr/local/cuda` as a symlink and `/usr/local/cuda-<version>` as the actual installation directory.

## Post-Installation Verification

### Verify NVIDIA Driver

```
nvidia-smi
```

Expected output: GPU information, driver version, CUDA version compatibility. If missing then NVIDIA drivers may not be installed - see [Step 1: Install NVIDIA GPU Drivers](#step-1-install-nvidia-gpu-drivers).

### Verify CUDA Compiler (nvcc)

```
nvcc --version
```

Expected output: CUDA compilation tools release version.

## Building CUDA Benchmarks

CUDA implementations of benchmarks will be built if the following conditions are met:

* NVIDIA GPU drivers are installed;
* CUDA Toolkit is installed;
* the CMake option `BUILD_CUDA` is set to `ON`;

If all environment variables and paths have been set properly (such as `CUDA_HOME`, `CUDA_PATH`, and `PATH` configurations from the installation steps above), no additional CMake flags are needed. CMake will automatically detect your CUDA installation.

### Example

```bash
git clone --recurse-submodules https://github.com/intel/compute-benchmarks
cd compute-benchmarks
mkdir build
cd build
cmake .. -DBUILD_CUDA=ON
cmake --build . --config Release
```

The last line can be substituted with platform-specific command, for example:
```bash
make -j`nproc`
```

If CMake cannot find your CUDA installation, you may need to specify the CUDA toolkit path explicitly:

**Linux:**
```bash
cmake .. -DBUILD_CUDA=ON -DCUDA_TOOLKIT_ROOT_DIR=/usr/local/cuda
```

**Windows:**
```cmd
cmake .. -DCMAKE_CUDA_COMPILER="C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v13.0/bin/nvcc.exe" -DBUILD_CUDA=ON -DCMAKE_GENERATOR_TOOLSET="cuda=C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v13.0"
```

Replace `v13.0` with your installed CUDA version.

## Troubleshooting

### Common Issues and Solutions

#### Issue 1: `nvcc: command not found` (Linux)

**Solution**: CUDA bin directory is not in PATH
```bash
export PATH=/usr/local/cuda/bin:$PATH
# Add to ~/.bashrc or ~/.zshrc for persistence
```

#### Issue 2: Visual Studio Cannot Find CUDA Build Customizations (Windows)

**Solution**: The recommended approach is to reinstall the CUDA Toolkit with Visual Studio Integration enabled (see Step 2d of [Windows Installation](#windows-installation)).

Alternatively, verify that integration files exist in:
```
C:\Program Files (x86)\Microsoft Visual Studio\<Year>\<Edition>\MSBuild\Microsoft\VC\v<Version>\BuildCustomizations
```
---

#### Issue 3: Error while loading shared libraries: libcuda.so.1

**Solution**: NVIDIA driver is not installed or not properly configured. Refer to [Step 1: Install NVIDIA GPU Drivers](#step-1-install-nvidia-gpu-drivers) for installation instructions.

If drivers are already installed but you're still experiencing issues, try reinstalling them.
