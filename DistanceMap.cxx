/*=========================================================================

  Program:   3D Slicer DistanceMap CLI
  Language:  C++
  Date:      $Date: 2009-04-25 12:27:26 $
  Version:   $Revision: 1.9 $

  Copyright (c) Brigham and Women's Hospital. All rights reserved.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#ifdef __BORLANDC__
#define ITK_LEAN_AND_MEAN
#endif

#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkCastImageFilter.h"

#include "itkDanielssonDistanceMapImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"

#include "itkPluginUtilities.h"
#include "DistanceMapCLP.h"

#include "itkMultiplyByConstantImageFilter.h"
#include "itkScaleVectorImageFilter.h"


namespace {

template<class T> int DoIt( int argc, char * argv[], T )
{
  PARSE_ARGS;

  const     unsigned int        Dimension       = 3;

  typedef   T                   InputPixelType;
  typedef   float               OutputPixelType;

  typedef   itk::Image< InputPixelType, Dimension > InputImageType;
  typedef   itk::Image< OutputPixelType, Dimension >    OutputImageType;

  typedef   itk::ImageFileReader< InputImageType  >  ReaderType;
  typedef   itk::ImageFileWriter< OutputImageType > WriterType;

  typedef   itk::DanielssonDistanceMapImageFilter<
               InputImageType, OutputImageType >  DistanceMapFilterType;

  typedef   typename DistanceMapFilterType::VectorImageType VectorImageType;

  typedef   itk::ScaleVectorImageFilter<VectorImageType, OutputImageType> ScaleVectorImageType;

  typename ReaderType::Pointer reader = ReaderType::New();  
  typename WriterType::Pointer writer = WriterType::New();
  typename WriterType::Pointer voronoiWriter = WriterType::New();
  typename DistanceMapFilterType::Pointer filter = DistanceMapFilterType::New();
  typename ScaleVectorImageType::Pointer scaler = ScaleVectorImageType::New();

  reader->SetFileName( inputVolume.c_str() );
  writer->SetFileName( outputVolume.c_str() );

  filter->SetInput( reader->GetOutput() );
  filter->InputIsBinaryOn();

  try
    {
    filter->Update();
    }
  catch (itk::ExceptionObject &err)
    {
    std::cerr << err << std::endl;
    return EXIT_FAILURE ;
    }

  // itk::DanielssonDistanceMapImageFilter does not take the pixel spacing into
  // account. If the physicalDistance flag is on, we first obtain vector map,
  // and then recalculate the physical distance map.
  // See: http://www.itk.org/pipermail/insight-users/2003-October/005320.html
  if (physicalDistance)
    {
    // Obtain pixel size
    typename ScaleVectorImageType::VectorType scale;
    typename InputImageType::SpacingType spacing = filter->GetOutput()->GetSpacing();
    for (unsigned int i = 0; i < Dimension; i ++)
      {
      scale[i] = spacing[i];
      }
    std::cout << "original scale = ("
              << scale[0] << ", "
              << scale[1] << ", "
              << scale[2] << ")" << std::endl;
    scaler->SetScaleVector (scale);
    scaler->SetInput( filter->GetVectorDistanceMap() );
    writer->SetInput( scaler->GetOutput() );
    }
  else
    {
    writer->SetInput( filter->GetOutput() );
    }
  
  try
    {
    writer->Update();
    }
  catch (itk::ExceptionObject &err)
    {
    std::cerr << err << std::endl;
    return EXIT_FAILURE ;
    }

  return EXIT_SUCCESS;
}

} // end of anonymous namespace


int main( int argc, char * argv[] )
{
  PARSE_ARGS;

  itk::ImageIOBase::IOPixelType pixelType;
  itk::ImageIOBase::IOComponentType componentType;

  try
    {
    itk::GetImageType (inputVolume, pixelType, componentType);

    // This filter handles all types on input, but only produces
    // signed types
    switch (componentType)
      {
      case itk::ImageIOBase::UCHAR:
        return DoIt( argc, argv, static_cast<unsigned char>(0));
        break;
      case itk::ImageIOBase::CHAR:
        return DoIt( argc, argv, static_cast<char>(0));
        break;
      case itk::ImageIOBase::USHORT:
        return DoIt( argc, argv, static_cast<unsigned short>(0));
        break;
      case itk::ImageIOBase::SHORT:
        return DoIt( argc, argv, static_cast<short>(0));
        break;
      case itk::ImageIOBase::UINT:
        return DoIt( argc, argv, static_cast<unsigned int>(0));
        break;
      case itk::ImageIOBase::INT:
        return DoIt( argc, argv, static_cast<int>(0));
        break;
      case itk::ImageIOBase::ULONG:
        return DoIt( argc, argv, static_cast<unsigned long>(0));
        break;
      case itk::ImageIOBase::LONG:
        return DoIt( argc, argv, static_cast<long>(0));
        break;
      case itk::ImageIOBase::FLOAT:
        return DoIt( argc, argv, static_cast<float>(0));
        break;
      case itk::ImageIOBase::DOUBLE:
        return DoIt( argc, argv, static_cast<double>(0));
        break;
      case itk::ImageIOBase::UNKNOWNCOMPONENTTYPE:
      default:
        std::cout << "unknown component type" << std::endl;
        break;
      }
    }

  catch( itk::ExceptionObject &excep)
    {
    std::cerr << argv[0] << ": exception caught !" << std::endl;
    std::cerr << excep << std::endl;
    return EXIT_FAILURE;
    }
  return EXIT_SUCCESS;
}
