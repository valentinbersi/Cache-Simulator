import subprocess
import pytest

@pytest.mark.it('Verifies that C is power of 2')
def test_C():
    args = ['./cachesim','empty.xex','2047','2','64']

    result = subprocess.run(args, capture_output=True, text=True)
    exit_code = result.returncode
    stderr = result.stderr.strip()

    assert exit_code != 0
    assert stderr != ''

@pytest.mark.it('Verifies that E is power of 2')
def test_E():
    args = ['./cachesim','empty.xex','2048','9','64']

    result = subprocess.run(args, capture_output=True, text=True)
    exit_code = result.returncode
    stderr = result.stderr.strip()

    assert exit_code != 0
    assert stderr != ''

@pytest.mark.it('Verifies that S is power of 2')
def test_S():
    args = ['./cachesim','empty.xex','2048','2','60']

    result = subprocess.run(args, capture_output=True, text=True)
    exit_code = result.returncode
    stderr = result.stderr.strip()

    assert exit_code != 0
    assert stderr != ''

@pytest.mark.it('Does not allow invalid ranges (n ≥ m)')
def test_invalid_n_m():
    args = ['./cachesim','empty.xex','2048','2','64','-v','200','150']

    result = subprocess.run(args, capture_output=True, text=True)
    exit_code = result.returncode
    stderr = result.stderr.strip()

    assert exit_code != 0
    assert stderr != ''

@pytest.mark.it('Emits an error in stderr for an invalid combination of C/E/S')
def test_invalid_CES():
    args = ['./cachesim','empty.xex','2048','32','256']

    result = subprocess.run(args, capture_output=True, text=True)
    exit_code = result.returncode
    stderr = result.stderr.strip()

    assert exit_code != 0
    assert stderr != ''


