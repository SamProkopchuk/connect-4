from setuptools import setup

with open('README.md', 'r') as f:
    long_description = f.read()

setup(
    name='connect-4',
    version='0.0.1',
    packages=['connect_4'],
    license='',
    url='https://github.com/SamProkopchuk/connect-4',
    author='SamProkopchuk',
    author_email='ssprokopchuk@gmail.com',
    description='An environment to test various Connect-4 AIs',
    install_requires=['numpy>=1.18.5'],
    extras_require = {
        'dev': ['pytest>=3.7']
    }
    classifiers=[
        'Programming Language :: Python :: 3',
        'License :: OSI Approved :: MIT License',
        'Operating System :: OS Independent',
    ],
    python_requires='>=3.8',
    long_description=long_description,
    long_description_content_type='text/markdown'
)
